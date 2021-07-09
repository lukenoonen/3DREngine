#include "DirLightShadow.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "TextureManager.h"

CConInt ci_r_dirshadowsize( "r_dirshadowsize", 2048 );

CDirLightShadow::CDirLightShadow( float flBlurRadius, float flLength, float flZNear, float flZFar, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	flLength /= 2.0f;

	m_flBlurScale = flBlurRadius / (flLength * M_SQRT2);

	m_matOrthoMatrix = glm::ortho( -flLength, flLength, -flLength, flLength, flZNear, flZFar );
	m_matLightSpaceMatrix = m_matOrthoMatrix * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );

	CreateShadowBuffers();
}

CDirLightShadow::~CDirLightShadow()
{
	DestroyShadowBuffers();
}

bool CDirLightShadow::CastShadows( void ) const
{
	return true;
}

void CDirLightShadow::PostThink( void )
{
	if (HasTransformUpdated())
	{
		m_matLightSpaceMatrix = m_matOrthoMatrix * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );

		ResetTransformUpdated();
	}
}

void CDirLightShadow::CalculateShadows( void )
{
	if (ci_r_dirshadowsize.WasDispatched())
	{
		pTextureManager->UnbindTexture( m_uiShadowMap );

		DestroyShadowBuffers();
		CreateShadowBuffers();
	}

	pRenderManager->SetViewportSize( glm::ivec2( ci_r_dirshadowsize.GetValue() ) );

	pShaderManager->SetShaderSubType( SHADERSUBTYPE_DIR );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatrix );

	pRenderManager->DrawShadowMap( m_uiShadowMapFBO );
}

void CDirLightShadow::ActivateLight( void )
{
	BaseClass::ActivateLight();

	float flNear = 100.0f;
	float flFar = 120.0f;

	pRenderManager->SetShadowMapIndex( pTextureManager->BindTexture( m_uiShadowMap, GL_TEXTURE_2D ), 0 );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatrix );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWBLUR, 0, 0, 1, &m_flBlurScale );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 0, &flNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 1, &flFar );
}

void CDirLightShadow::CreateShadowBuffers( void )
{
	int iShadowSize = ci_r_dirshadowsize.GetValue();

	glGenFramebuffers( 1, &m_uiShadowMapFBO );
	glGenTextures( 1, &m_uiShadowMap );

	glBindTexture( GL_TEXTURE_2D, m_uiShadowMap );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, iShadowSize, iShadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
	float flBorderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flBorderColor );
	glBindFramebuffer( GL_FRAMEBUFFER, m_uiShadowMapFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uiShadowMap, 0 );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
}

void CDirLightShadow::DestroyShadowBuffers( void )
{
	glDeleteFramebuffers( 1, &m_uiShadowMapFBO );
	glDeleteTextures( 1, &m_uiShadowMap );
}