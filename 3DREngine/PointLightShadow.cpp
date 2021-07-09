#include "PointLightShadow.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "TextureManager.h"

CConInt ci_r_pntshadowsize( "r_pntshadowsize", 512 );

CPointLightShadow::CPointLightShadow( float flBlurRadius, float flConstant, float flLinear, float flQuadratic, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( flConstant, flLinear, flQuadratic, vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flBlurScale = flBlurRadius / M_SQRT2;

	m_matProjectionMatrix = glm::perspective( 2.0f * atanf( (1.0f + m_flBlurScale * 2.0f) ), 1.0f, 0.1f, GetMaxRadius() );
	m_matLightSpaceMatricies[0] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecRight, g_vecBack );
	m_matLightSpaceMatricies[1] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecLeft, g_vecBack );
	m_matLightSpaceMatricies[2] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecFront, g_vecUp );
	m_matLightSpaceMatricies[3] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecBack, g_vecDown );
	m_matLightSpaceMatricies[4] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecUp, g_vecBack );
	m_matLightSpaceMatricies[5] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecDown, g_vecBack );

	CreateShadowBuffers();
}

CPointLightShadow::~CPointLightShadow()
{
	DestroyShadowBuffers();
}

bool CPointLightShadow::CastShadows( void ) const
{
	return true;
}

void CPointLightShadow::PostThink( void )
{
	if (HasTransformUpdated())
	{
		m_matLightSpaceMatricies[0] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecFront, g_vecUp );
		m_matLightSpaceMatricies[1] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecRight, g_vecUp );
		m_matLightSpaceMatricies[2] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecBack, g_vecUp );
		m_matLightSpaceMatricies[3] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecLeft, g_vecUp );
		m_matLightSpaceMatricies[4] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecUp, g_vecFront );
		m_matLightSpaceMatricies[5] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecDown, g_vecFront );

		ResetTransformUpdated();
	}
}

void CPointLightShadow::CalculateShadows( void )
{
	if (ci_r_pntshadowsize.WasDispatched())
	{
		pTextureManager->UnbindTexture( m_uiShadowMap );

		DestroyShadowBuffers();
		CreateShadowBuffers();
	}

	float flMaxRadius = GetMaxRadius();

	pRenderManager->SetViewportSize( glm::ivec2( ci_r_pntshadowsize.GetValue() ) );

	pShaderManager->SetShaderSubType( SHADERSUBTYPE_POINT );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOSITION, 0, &GetPosition() );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTMAXDISTANCE, 0, &flMaxRadius );

	int iShadowSize = ci_r_pntshadowsize.GetValue();

	glBindFramebuffer( GL_FRAMEBUFFER, m_uiShadowMapFBO );
	glClear( GL_DEPTH_BUFFER_BIT );

	for (unsigned int i = 0; i < 6; i++)
	{
		pRenderManager->SetViewportOffset( glm::vec2( iShadowSize * i, 0 ) );
		pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatricies[i] );
		pRenderManager->DrawEntities();
	}

	pRenderManager->SetViewportOffset( g_vec2Zero );
}

void CPointLightShadow::ActivateLight( void )
{
	BaseClass::ActivateLight();

	float flNear = 40.0f;
	float flFar = 50.0f;

	pRenderManager->SetShadowMapIndex( pTextureManager->BindTexture( m_uiShadowMap, GL_TEXTURE_2D ), 0 );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 6, m_matLightSpaceMatricies );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWBLUR, 0, 0, 1, &m_flBlurScale );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 0, &flNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 1, &flFar );
}

void CPointLightShadow::CreateShadowBuffers( void )
{
	int iShadowSize = ci_r_pntshadowsize.GetValue();

	glGenFramebuffers( 1, &m_uiShadowMapFBO );
	glGenTextures( 1, &m_uiShadowMap );

	glBindTexture( GL_TEXTURE_2D, m_uiShadowMap );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, iShadowSize * 6, iShadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS );
	glBindFramebuffer( GL_FRAMEBUFFER, m_uiShadowMapFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uiShadowMap, 0 );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
}

void CPointLightShadow::DestroyShadowBuffers( void )
{
	glDeleteFramebuffers( 1, &m_uiShadowMapFBO );
	glDeleteTextures( 1, &m_uiShadowMap );
}