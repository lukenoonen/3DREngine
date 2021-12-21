#include "CSMLight.h"
#include "ShaderManager.h"
#include "EntityManager.h"
#include "RenderManager.h"
#include "CommandManager.h"
#include "AssetManager.h"

CCSMLight::CCSMLight( CCSMShadowCamera *pCSMShadowCamera, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( pCSMShadowCamera, vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{

}
/*
void CCSMLight::CalculateShadows( void )
{
	if (ci_r_csmshadowsize.WasDispatched())
	{
		for (unsigned int i = 0; i < 4; i++)
			pAssetManager->UnbindTexture( m_uiShadowMaps[i] );

		pRenderManager->DestroyShadowMapFrameBuffer( 4, m_uiShadowMapFBOs, m_uiShadowMaps );

		int iShadowSize = ci_r_csmshadowsize.GetValue();
		pRenderManager->CreateShadowMapFramebuffer( 4, m_uiShadowMapFBOs, m_uiShadowMaps, iShadowSize, iShadowSize );
	}

	CPlayer *pPlayer = pEntityManager->GetPlayer();

	if (pPlayer->PositionUpdated() || pPlayer->RotationUpdated() || ci_r_csmshadowsize.WasDispatched() || RotationUpdated())
	{
		CalculateCascade();
		CalculateRadius();

		float flShadowSize = (float)ci_r_csmshadowsize.GetValue();
		glm::mat4 matLightView = glm::lookAt( pPlayer->GetPosition() - GetRotation() * g_vecFront, pPlayer->GetPosition(), GetRotation() * g_vecUp );

		for (unsigned int i = 0; i < 4; i++)
		{
			glm::vec3 vecFrustumCenter = pPlayer->GetPosition() + pPlayer->GetFront() * ((m_flCascadeEnd[i + 1] + m_flCascadeEnd[i]) * 0.5f);
			glm::vec3 maxOrtho = glm::vec3( matLightView * glm::vec4( vecFrustumCenter, 1.0f ) ) + glm::vec3( m_flRadius[i] );
			glm::vec3 minOrtho = glm::vec3( matLightView * glm::vec4( vecFrustumCenter, 1.0f ) ) - glm::vec3( m_flRadius[i] );

			glm::mat4 matLightOrtho = glm::ortho( minOrtho.x, maxOrtho.x, minOrtho.y, maxOrtho.y, minOrtho.z + m_flNearZError, maxOrtho.z + m_flFarZError );
			glm::vec4 shadowOrigin = ((matLightOrtho * matLightView) * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f )) * flShadowSize * 0.5f;
			glm::vec4 roundOffset = glm::round( shadowOrigin ) - shadowOrigin;
			roundOffset = roundOffset *  2.0f / flShadowSize;
			roundOffset.z = 0.0f;
			roundOffset.w = 0.0f;
			matLightOrtho[3] += roundOffset;

			m_matLightSpaceMatricies[i] = matLightOrtho * matLightView;
		}
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
	pRenderManager->SetRenderPass( RENDERPASS_SHADOW_CSM );

	for (unsigned int i = 0; i < 4; i++)
	{
		pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatricies[i] );
		pRenderManager->DrawShadowMap( m_uiShadowMapFBOs[i] );
	}
}*/

void CCSMLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( RENDERPASS_LIT_CSM );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTDIRECTION, 0, &(GetRotation() * g_vecFront) );
}
/*
void CCSMLight::CalculateCascade( void )
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

void CCSMLight::CalculateRadius( void )
{
	for (unsigned int i = 0; i < 4; i++)
	{
		float farHeight = tanf( glm::radians( cf_r_fov.GetValue() ) * 0.5f ) * m_flCascadeEnd[i + 1];
		float farWidth = farHeight * pRenderManager->GetWindowSizeRatio();
		m_flRadius[i] = glm::sqrt( m_flCascadeEnd[i + 1] * m_flCascadeEnd[i + 1] + farHeight * farHeight + farWidth * farWidth );
	}

	m_flBlurScale = m_flBlurRadius / (m_flRadius[0] * 2.0f);
}*/