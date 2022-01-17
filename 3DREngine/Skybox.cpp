#include "Skybox.h"
#include "RenderManager.h"
#include "AssetManager.h"

CSkybox::CSkybox( const char *sMaterialPath ) : BaseClass()
{
	m_pMaterial = pAssetManager->GetMaterial( sMaterialPath );
	m_pGeometry = pAssetManager->GetGeometry( "inversecube.3gm" );

	m_pMaterial->Activate();
	m_pGeometry->Activate();
}

CSkybox::~CSkybox()
{
	m_pMaterial->Inactivate();
	m_pGeometry->Inactivate();

	pAssetManager->CheckMaterial( m_pMaterial );
	pAssetManager->CheckGeometry( m_pGeometry );
}

void CSkybox::PreDraw( void )
{
	m_pMaterial->Use();
}

void CSkybox::Draw( void )
{
	m_pGeometry->Draw();
}

bool CSkybox::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_pMaterial->ShouldDraw();
}