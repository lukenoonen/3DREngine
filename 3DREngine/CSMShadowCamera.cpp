#include "CSMShadowCamera.h"
#include "CommandManager.h"
#include "Player.h"
#include "EntityManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "AssetManager.h"

CCSMShadowCamera::CCSMShadowCamera( float flBlendDistance, float flDistanceFactor, float flInitialDistance, float flNearZError, float flFarZError, float flFadeNear, float flFadeFar, float flBlurRadius, unsigned int uiBaseSize, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( flFadeNear, flFadeFar, 0.0f, uiBaseSize, 4.0f, uiRenderPriority, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flBlendDistance = flBlendDistance;
	m_flDistanceFactor = flDistanceFactor;
	m_flInitialDistance = flInitialDistance;
	m_flNearZError = flNearZError;
	m_flFarZError = flFarZError;

	m_flBlurRadius = flBlurRadius;

	CalculateCascade();
	CalculateRadius();
	CalculateTotal();
	CalculateNearFar();

	CreateTextureBuffers();
}

CCSMShadowCamera::~CCSMShadowCamera()
{
	DestroyTextureBuffers();
}

void CCSMShadowCamera::PostThink()
{
	CPlayer *pPlayer = pEntityManager->GetPlayer();
	bool bCalculateTotal = pPlayer->PositionUpdated() || pPlayer->RotationUpdated() || RotationUpdated() || cf_r_vcsizefactor.WasDispatched();

	if (cf_r_near.WasDispatched())
	{
		CalculateCascade();
		CalculateRadius();
		CalculateNearFar();
		bCalculateTotal = true;
	}
	else
	{
		if (cf_r_fov.WasDispatched() || cv_r_windowsize.WasDispatched())
		{
			CalculateRadius();
			bCalculateTotal = true;
		}

		if (cf_r_far.WasDispatched())
		{
			CalculateNearFar();
		}
	}

	if (bCalculateTotal)
	{
		CalculateTotal();
	}
}

void CCSMShadowCamera::Render( void )
{
	glm::vec2 vecSize = glm::ivec2( GetSize().y );

	pRenderManager->SetViewportSize( vecSize );
	pRenderManager->SetFrameBuffer( m_uiTextureFBO );

	pRenderManager->SetRenderPass( RENDERPASS_SHADOW_CSM );

	for (unsigned int i = 0; i < 4; i++)
	{
		pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matTotal[i] );
		pRenderManager->SetViewportOffset( glm::ivec2( vecSize.x * i, 0 ) );
		pRenderManager->DrawNonLitEntities();
	}

	pRenderManager->SetViewportOffset( g_vec2Zero );
}

void CCSMShadowCamera::CreateTextureBuffers( void )
{
	const glm::ivec2 &vecSize = GetSize();

	glGenFramebuffers( 1, &m_uiTextureFBO );
	glGenTextures( 1, &m_uiTexture );

	glBindTexture( GL_TEXTURE_2D, m_uiTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, vecSize.x, vecSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
	glBindFramebuffer( GL_FRAMEBUFFER, m_uiTextureFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uiTexture, 0 );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
}

void CCSMShadowCamera::DestroyTextureBuffers( void )
{
	pAssetManager->UnbindTexture( m_uiTexture );

	glDeleteFramebuffers( 1, &m_uiTextureFBO );
	glDeleteTextures( 1, &m_uiTexture );
}

void CCSMShadowCamera::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetShadowMapIndex( pAssetManager->BindTexture( m_uiTexture, GL_TEXTURE_2D ) );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 4, m_matTotal );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWCASCADEFADE, 0, &m_vecCascadeEndClipSpaceNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWCASCADEFADE, 1, &m_vecCascadeEndClipSpaceFar );
}

void CCSMShadowCamera::CalculateCascade( void )
{
	float flBlendDistance = m_flBlendDistance;
	float flDistanceFactor = m_flDistanceFactor;

	m_flCascadeEnd[0] = cf_r_near.GetValue();
	float flDistance = m_flInitialDistance + m_flCascadeEnd[0];
	for (unsigned int i = 1; i < 5; i++)
	{
		m_flCascadeEnd[i] = flDistance;
		m_flCascadeEndNear[i - 1] = flDistance - flBlendDistance;
		flDistance *= flDistanceFactor;
		flBlendDistance *= flDistanceFactor;
	}
}

void CCSMShadowCamera::CalculateRadius( void )
{
	for (unsigned int i = 0; i < 4; i++)
	{
		float farHeight = tanf( glm::radians( cf_r_fov.GetValue() ) * 0.5f ) * m_flCascadeEnd[i + 1];
		float farWidth = farHeight * cv_r_windowsize.GetValue().x / cv_r_windowsize.GetValue().y;
		m_flRadius[i] = glm::sqrt( m_flCascadeEnd[i + 1] * m_flCascadeEnd[i + 1] + farHeight * farHeight + farWidth * farWidth );
	}

	SetBlurScale( m_flBlurRadius * 0.5f / m_flRadius[0] );
}

void CCSMShadowCamera::CalculateTotal( void )
{
	CPlayer *pPlayer = pEntityManager->GetPlayer();

	float flShadowSize = (float)GetSize().y;

	glm::mat4 matLightView = glm::lookAt( pPlayer->GetPosition() - GetRotation() * g_vecFront, pPlayer->GetPosition(), GetRotation() * g_vecUp );

	for (unsigned int i = 0; i < 4; i++)
	{
		glm::vec3 vecFrustumCenter = pPlayer->GetPosition() + pPlayer->GetRotation() * g_vecFront * ((m_flCascadeEnd[i + 1] + m_flCascadeEnd[i]) * 0.5f);
		glm::vec3 maxOrtho = glm::vec3( matLightView * glm::vec4( vecFrustumCenter, 1.0f ) ) + glm::vec3( m_flRadius[i] );
		glm::vec3 minOrtho = glm::vec3( matLightView * glm::vec4( vecFrustumCenter, 1.0f ) ) - glm::vec3( m_flRadius[i] );

		glm::mat4 matLightOrtho = glm::ortho( minOrtho.x, maxOrtho.x, minOrtho.y, maxOrtho.y, minOrtho.z + m_flNearZError, maxOrtho.z + m_flFarZError );
		glm::vec4 vecShadowOrigin = ((matLightOrtho * matLightView) * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f )) * flShadowSize * 0.5f;
		glm::vec4 vecRoundOffset = glm::round( vecShadowOrigin ) - vecShadowOrigin;
		vecRoundOffset = vecRoundOffset * 2.0f / flShadowSize;
		vecRoundOffset.z = 0.0f;
		vecRoundOffset.w = 0.0f;
		matLightOrtho[3] += vecRoundOffset;

		m_matTotal[i] = matLightOrtho * matLightView;
	}
}

void CCSMShadowCamera::CalculateNearFar( void )
{
	for (unsigned int i = 0; i < 4; i++)
	{
		float flNear = cf_r_near.GetValue();
		float flFar = cf_r_far.GetValue();

		float flFarPlusNear = flFar + flNear;
		float flTwoTimesFarTimesNear = 2.0f * flFar * flNear;
		float flInverseFarMinusNear = 1.0f / (flFar - flNear);

		m_vecCascadeEndClipSpaceNear[i] = (m_flCascadeEndNear[i] * flFarPlusNear - flTwoTimesFarTimesNear) * flInverseFarMinusNear;
		m_vecCascadeEndClipSpaceFar[i] = (m_flCascadeEnd[i + 1] * flFarPlusNear - flTwoTimesFarTimesNear) * flInverseFarMinusNear;
	}
}