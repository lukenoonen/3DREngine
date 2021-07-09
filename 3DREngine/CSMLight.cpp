#include "CSMLight.h"
#include "ShaderManager.h"
#include "EntityManager.h"
#include "RenderManager.h"
#include "TextureManager.h"

CConInt ci_r_csmshadowsize( "r_csmshadowsize", 1024 );

CCSMLight::CCSMLight( float flBlurRadius, float flBlendDistance, float flInitialDistance, float flDistanceFactor, float flFarZError, float flNearZError, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flBlurRadius = flBlurRadius;

	m_flFarZError = flFarZError;
	m_flNearZError = flNearZError;
	m_flCascadeEnd[0] = cf_r_near.GetValue();
	float flDistance = flInitialDistance + m_flCascadeEnd[0];
	for (unsigned int i = 1; i < 5; i++)
	{
		m_flCascadeEnd[i] = flDistance;
		m_flCascadeEndNear[i - 1] = flDistance - flBlendDistance;
		flDistance *= flDistanceFactor;
		flBlendDistance *= flDistanceFactor;
	}

	CreateShadowBuffers();
}

CCSMLight::~CCSMLight()
{
	DestroyShadowBuffers();
}

bool CCSMLight::CastShadows( void ) const
{
	return true;
}

void CCSMLight::CalculateShadows( void )
{
	if (ci_r_csmshadowsize.WasDispatched())
	{
		for (unsigned int i = 0; i < 4; i++)
			pTextureManager->UnbindTexture( m_uiShadowMaps[i] );

		DestroyShadowBuffers();
		CreateShadowBuffers();
	}

	CPlayer *pPlayer = pEntityManager->GetPlayer();

	if (pPlayer->HasViewChanged() || ci_r_csmshadowsize.WasDispatched() || HasTransformUpdated())
	{
		float flShadowSize = (float)ci_r_csmshadowsize.GetValue();

		for (unsigned int i = 0; i < 4; i++)
		{
			float farWidth = tanf( cf_r_fov.GetValue() / 2.0f ) * m_flCascadeEnd[i + 1];
			float farHeight = farWidth / pRenderManager->GetWindowSizeRatio();
			glm::vec3 vecFrustumCenter = pPlayer->GetPosition() + pPlayer->GetFront() * ((m_flCascadeEnd[i + 1] - m_flCascadeEnd[i]) / 2.0f);
			glm::mat4 matLightView = glm::lookAt( vecFrustumCenter - (GetRotation() * g_vecFront), vecFrustumCenter, GetRotation() * g_vecUp );
			float flRadius = glm::length( vecFrustumCenter - (pPlayer->GetPosition() + pPlayer->GetFront() * m_flCascadeEnd[i + 1] + (pPlayer->GetUp() * farHeight) + (pPlayer->GetRight() * farWidth)) );

			glm::vec3 maxOrtho = glm::vec3( matLightView * glm::vec4( vecFrustumCenter, 1.0f ) ) + glm::vec3( flRadius );
			glm::vec3 minOrtho = glm::vec3( matLightView * glm::vec4( vecFrustumCenter, 1.0f ) ) - glm::vec3( flRadius );

			glm::mat4 matLightOrtho = glm::ortho( minOrtho.x, maxOrtho.x, minOrtho.y, maxOrtho.y, minOrtho.z + m_flNearZError, maxOrtho.z + m_flFarZError );
			glm::vec4 shadowOrigin = ((matLightOrtho * matLightView) * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f )) * flShadowSize / 2.0f;
			glm::vec4 roundOffset = glm::round( shadowOrigin ) - shadowOrigin;
			roundOffset = roundOffset *  2.0f / flShadowSize;
			roundOffset.z = 0.0f;
			roundOffset.w = 0.0f;
			matLightOrtho[3] += roundOffset;

			m_matLightSpaceMatricies[i] = matLightOrtho * matLightView;
			m_vecCascadeBlurScale[i] = m_flBlurRadius / flRadius;
		}

		ResetTransformUpdated();
	}

	if (pPlayer->HasProjectionChanged())
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::vec4 vecView( 0.0f, 0.0f, -m_flCascadeEndNear[i], 1.0f );
			glm::vec4 vecClip = pEntityManager->GetPlayer()->GetProjection() * vecView;
			m_vecCascadeEndClipSpaceNear[i] = vecClip.z;
		}

		for (unsigned int i = 0; i < 4; i++)
		{
			glm::vec4 vecView( 0.0f, 0.0f, -m_flCascadeEnd[i + 1], 1.0f );
			glm::vec4 vecClip = pEntityManager->GetPlayer()->GetProjection() * vecView;
			m_vecCascadeEndClipSpaceFar[i] = vecClip.z;
		}
	}

	pRenderManager->SetViewportSize( glm::ivec2( ci_r_csmshadowsize.GetValue() ) );

	pShaderManager->SetShaderSubType( SHADERSUBTYPE_CSM );

	for (unsigned int i = 0; i < 4; i++)
	{
		pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatricies[i] );
		pRenderManager->DrawShadowMap( m_uiShadowMapFBOs[i] );
	}
}

void CCSMLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	float flNear = 280.0f;
	float flFar = 300.0f;

	pShaderManager->SetShaderSubType( SHADERSUBTYPE_CSM );

	for (unsigned int i = 0; i < 4; i++)
		pRenderManager->SetShadowMapIndex( pTextureManager->BindTexture( m_uiShadowMaps[i], GL_TEXTURE_2D ), i );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTDIRECTION, 0, &(GetRotation() * g_vecFront) );
	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 4, m_matLightSpaceMatricies );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWBLUR, 0, 0, 4, &m_vecCascadeBlurScale[0] );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWCASCADEFADE, 0, &m_vecCascadeEndClipSpaceNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWCASCADEFADE, 1, &m_vecCascadeEndClipSpaceFar );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 0, &flNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 1, &flFar );
}

void CCSMLight::CreateShadowBuffers( void )
{
	int iShadowSize = ci_r_csmshadowsize.GetValue();

	glGenFramebuffers( 4, m_uiShadowMapFBOs );
	glGenTextures( 4, m_uiShadowMaps );

	for (unsigned int i = 0; i < 4; i++)
	{
		glBindTexture( GL_TEXTURE_2D, m_uiShadowMaps[i] );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, iShadowSize, iShadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
		float flBorderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flBorderColor );
		glBindFramebuffer( GL_FRAMEBUFFER, m_uiShadowMapFBOs[i] );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uiShadowMaps[i], 0 );
		glDrawBuffer( GL_NONE );
		glReadBuffer( GL_NONE );
	}
}

void CCSMLight::DestroyShadowBuffers( void )
{
	glDeleteFramebuffers( 4, m_uiShadowMapFBOs );
	glDeleteTextures( 4, m_uiShadowMaps );
}