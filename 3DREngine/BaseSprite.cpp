#include "BaseSprite.h"
#include "RenderManager.h"
#include "EntityManager.h"

/*
CBaseSprite::CBaseSprite()
{
	m_pMaterial = NULL;

	m_pGeometry = pAssetManager->GetGeometry( "plane.3gm" );
	m_pGeometry->Activate();
}

bool CBaseSprite::Exit( void )
{
	if (m_pMaterial)
	{
		m_pMaterial->Inactivate();
		pAssetManager->CheckGeometry( m_pGeometry );
	}

	m_pGeometry->Inactivate();
	pAssetManager->CheckMaterial( m_pMaterial );

	return BaseClass::Exit();
}

void CBaseSprite::PostThink( void )
{
	CBaseCamera *pCamera = pEntityManager->GetPlayer()->GetCamera();

	SetRotation( glm::quat( glm::eulerAngles( pCamera->GetRotation() ) * glm::vec3( 0.0f, 0.0f, 0.0f ) ) * glm::quat( glm::radians( glm::vec3( 90.0f, 0.0f, 0.0f ) ) ) );

	BaseClass::PostThink();
}

void CBaseSprite::PreDraw( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 0, &GetModelMatrix() );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 1, &GetModelMatrixInverse() );
}

void CBaseSprite::Draw( void )
{
	m_pMaterial->Use();
	m_pGeometry->Draw();
}

bool CBaseSprite::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_pMaterial != NULL && m_pMaterial->ShouldDraw();
}

void CBaseSprite::SetMaterial( const char *sMaterialPath )
{
	if (m_pMaterial)
	{
		m_pMaterial->Inactivate();
		pAssetManager->CheckMaterial( m_pMaterial );
	}

	m_pMaterial = pAssetManager->GetMaterial( sMaterialPath );

	if (m_pMaterial)
		m_pMaterial->Activate();
}*/