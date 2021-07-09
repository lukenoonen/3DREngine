#include "SpotLightShadow.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "TextureManager.h"

CConInt ci_r_sptshadowsize( "r_sptshadowsize", 512 );

CSpotLightShadow::CSpotLightShadow( float flBlurRadius, float flCutoff, float flOuterCutoff, float flConstant, float flLinear, float flQuadratic, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( flCutoff, flOuterCutoff, flConstant, flLinear, flQuadratic, vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flBlurScale = flBlurRadius / (tanf( flOuterCutoff ) * M_SQRT2);

	m_matProjectionMatrix = glm::perspective( flOuterCutoff * 2.0f, 1.0f, 0.1f, GetMaxRadius() );
	m_matLightSpaceMatrix = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );

	CreateShadowBuffers();
}

CSpotLightShadow::~CSpotLightShadow()
{
	DestroyShadowBuffers();
}

bool CSpotLightShadow::CastShadows( void ) const
{
	return true;
}

void CSpotLightShadow::PostThink( void )
{
	if (HasTransformUpdated())
	{
		m_matLightSpaceMatrix = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
		ResetTransformUpdated();
	}
}

void CSpotLightShadow::CalculateShadows( void )
{
	if (ci_r_sptshadowsize.WasDispatched())
	{
		pTextureManager->UnbindTexture( m_uiShadowMap );

		DestroyShadowBuffers();
		CreateShadowBuffers();
	}

	float flMaxRadius = GetMaxRadius();

	pRenderManager->SetViewportSize( glm::ivec2( ci_r_sptshadowsize.GetValue() ) );

	pShaderManager->SetShaderSubType( SHADERSUBTYPE_SPOT );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatrix );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOSITION, 0, &GetPosition() );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTMAXDISTANCE, 0, &flMaxRadius );

	pRenderManager->DrawShadowMap( m_uiShadowMapFBO );
}

void CSpotLightShadow::ActivateLight( void )
{
	BaseClass::ActivateLight();

	float flNear = 40.0f;
	float flFar = 50.0f;

	pRenderManager->SetShadowMapIndex( pTextureManager->BindTexture( m_uiShadowMap, GL_TEXTURE_2D ), 0 );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatrix );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWBLUR, 0, 0, 1, &m_flBlurScale );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 0, &flNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 1, &flFar );
}

void CSpotLightShadow::CreateShadowBuffers( void )
{
	int iShadowSize = ci_r_sptshadowsize.GetValue();

	glGenFramebuffers( 1, &m_uiShadowMapFBO );
	glGenTextures( 1, &m_uiShadowMap );

	glBindTexture( GL_TEXTURE_2D, m_uiShadowMap );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, iShadowSize, iShadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
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

void CSpotLightShadow::DestroyShadowBuffers( void )
{
	glDeleteFramebuffers( 1, &m_uiShadowMapFBO );
	glDeleteTextures( 1, &m_uiShadowMap );
}