#include "CSMShadowCamera.h"
#include "CommandManager.h"
#include "BasePlayerCamera.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "AssetManager.h"

CCSMShadowCamera::CCSMShadowCamera()
{
	m_flBlendDistance = 6.0f;
	m_flDistanceFactor = 4.0f;
	m_flInitialDistance = 6.0f;
	m_flNearError = -60.0f;
	m_flFarError = 60.0f;
	m_flBlurRadius = 0.0f;

	SetSizeRatio( 4.0f );
}

void CCSMShadowCamera::Init( void )
{
	m_bUpdateCascade = false;
	m_bUpdateRadius = false;
	m_bUpdateTotal = false;
	m_bUpdateNearFar = false;
	m_bUpdateBlurScale = false;

	BaseClass::Init();

	CalculateCascade();
	CalculateRadius();
	CalculateTotal();
	CalculateNearFar();

	SetBlurScale( m_flBlurRadius * 0.5f / m_flRadius[0] );
}

void CCSMShadowCamera::PostThink( void )
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetPlayer()->GetCamera();
	m_bUpdateCascade = m_bUpdateCascade || cf_r_near.WasDispatched();
	m_bUpdateRadius = m_bUpdateCascade || cf_r_fov.WasDispatched() || cv_r_windowsize.WasDispatched();
	m_bUpdateTotal = m_bUpdateTotal || m_bUpdateRadius || pPlayerCamera->PositionUpdated() || pPlayerCamera->RotationUpdated() || RotationUpdated() || cf_r_vcsizefactor.WasDispatched(); // TODO: swap cf_r_vcsizefactor for on size change
	m_bUpdateNearFar = m_bUpdateNearFar || m_bUpdateCascade || cf_r_far.WasDispatched();
	m_bUpdateBlurScale = m_bUpdateBlurScale || m_bUpdateRadius;

	if (m_bUpdateCascade)
	{
		CalculateCascade();
		m_bUpdateCascade = false;
	}

	if (m_bUpdateRadius)
	{
		CalculateRadius();
		m_bUpdateRadius = false;
	}

	if (m_bUpdateTotal)
	{
		CalculateTotal();
		m_bUpdateTotal = false;
	}

	if (m_bUpdateNearFar)
	{
		CalculateNearFar();
		m_bUpdateNearFar = false;
	}

	if (m_bUpdateBlurScale)
	{
		SetBlurScale( m_flBlurRadius * 0.5f / m_flRadius[0] );
		m_bUpdateBlurScale = false;
	}

	BaseClass::PostThink();
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

		pEntityManager->DrawUnlitEntities();
	}

	pRenderManager->SetViewportOffset( g_vec2Zero );
}

void CCSMShadowCamera::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetShadowMapIndex( pAssetManager->BindTexture( m_uiTexture, GL_TEXTURE_2D ) );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 4, m_matTotal );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWCASCADEFADE, 0, &m_vecCascadeEndClipSpaceNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWCASCADEFADE, 1, &m_vecCascadeEndClipSpaceFar );
}

void CCSMShadowCamera::SetBlendDistance( float flBlendDistance )
{
	m_flBlendDistance = flBlendDistance;
	m_bUpdateCascade = true;
}

void CCSMShadowCamera::SetDistanceFactor( float flDistanceFactor )
{
	m_flDistanceFactor = flDistanceFactor;
	m_bUpdateCascade = true;
}

void CCSMShadowCamera::SetInitialDistance( float flInitialDistance )
{
	m_flInitialDistance = flInitialDistance;
	m_bUpdateCascade = true;
}

void CCSMShadowCamera::SetNearError( float flNearError )
{
	m_flNearError = flNearError;
	m_bUpdateTotal = true;
}

void CCSMShadowCamera::SetFarError( float flFarError )
{
	m_flFarError = flFarError;
	m_bUpdateTotal = true;
}

void CCSMShadowCamera::SetBlurRadius( float flBlurRadius )
{
	m_flBlurRadius = flBlurRadius;
	m_bUpdateBlurScale = true;
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

void CCSMShadowCamera::CalculateCascade( void )
{
	float flBlendDistance = m_flBlendDistance;

	m_flCascadeEnd[0] = cf_r_near.GetValue();
	float flDistance = m_flInitialDistance + m_flCascadeEnd[0];
	for (unsigned int i = 0; i < 4; i++)
	{
		m_flCascadeEnd[i + 1] = flDistance;
		m_flCascadeEndNear[i] = flDistance - flBlendDistance;

		flBlendDistance = (m_flDistanceFactor - 1.0f) * (flBlendDistance + m_flCascadeEnd[i]);
		flDistance *= m_flDistanceFactor;
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
}

void CCSMShadowCamera::CalculateTotal( void )
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetPlayer()->GetCamera();

	float flShadowSize = (float)GetSize().y;

	glm::mat4 matLightView = glm::lookAt( pPlayerCamera->GetPosition() - GetRotation() * g_vecFront, pPlayerCamera->GetPosition(), GetRotation() * g_vecUp );

	for (unsigned int i = 0; i < 4; i++)
	{
		glm::vec3 vecFrustumCenter = pPlayerCamera->GetPosition() + pPlayerCamera->GetRotation() * g_vecFront * ((m_flCascadeEnd[i + 1] + m_flCascadeEnd[i]) * 0.5f);
		glm::vec3 maxOrtho = glm::vec3( matLightView * glm::vec4( vecFrustumCenter, 1.0f ) ) + glm::vec3( m_flRadius[i] );
		glm::vec3 minOrtho = glm::vec3( matLightView * glm::vec4( vecFrustumCenter, 1.0f ) ) - glm::vec3( m_flRadius[i] );

		glm::mat4 matLightOrtho = glm::ortho( minOrtho.x, maxOrtho.x, minOrtho.y, maxOrtho.y, minOrtho.z + m_flNearError, maxOrtho.z + m_flFarError );
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