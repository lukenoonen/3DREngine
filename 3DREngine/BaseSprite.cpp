#include "BaseSprite.h"
#include "AssetManager.h"
#include "EntityManager.h"

CBaseSprite::CBaseSprite( const char *sMaterialPath )
{
	m_pMaterial = pAssetManager->GetMaterial( sMaterialPath );
	m_pGeometry = pAssetManager->GetGeometry( "plane.3gm" );

	m_pMaterial->Activate();
	m_pGeometry->Activate();
}

CBaseSprite::~CBaseSprite()
{
	m_pMaterial->Inactivate();
	m_pGeometry->Inactivate();

	pAssetManager->CheckMaterial( m_pMaterial );
	pAssetManager->CheckGeometry( m_pGeometry );
}

void CBaseSprite::PostThink( void )
{
	CBaseCamera *pCamera = pEntityManager->GetPlayer()->GetCamera();

	SetRotation( glm::quat( glm::eulerAngles( pCamera->GetRotation() ) * glm::vec3( 0.0f, 1.0f, 1.0f ) ) * glm::quat( glm::radians( glm::vec3( 90.0f, 0.0f, 0.0f ) ) ) );

	BaseClass::PostThink();
}

void CBaseSprite::PreDraw( void )
{
	pShaderManager->SetUniformBufferObject( UBO_MODEL, 0, &GetModelMatrix() );
	pShaderManager->SetUniformBufferObject( UBO_MODEL, 1, &GetModelMatrixInverse() );
}

void CBaseSprite::Draw( void )
{
	m_pMaterial->Use();
	m_pGeometry->Draw();
}

bool CBaseSprite::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_pMaterial->ShouldDraw();
}