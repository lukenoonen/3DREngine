#include "Skybox.h"
#include "RenderManager.h"
#include "AssetManager.h"

CSkybox::CSkybox()
{
	m_pMaterial = NULL;

	m_pGeometry = pAssetManager->GetGeometry( "inversecube.3gm" );
	m_pGeometry->Activate();
}

void CSkybox::Exit( void )
{
	if (m_pMaterial)
	{
		m_pMaterial->Inactivate();
		pAssetManager->CheckGeometry( m_pGeometry );
	}

	m_pGeometry->Inactivate();
	pAssetManager->CheckMaterial( m_pMaterial );
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
	return BaseClass::ShouldDraw() && m_pMaterial != NULL && m_pMaterial->ShouldDraw();
}

void CSkybox::SetMaterial( const char *sMaterialPath )
{
	if (m_pMaterial)
	{
		m_pMaterial->Inactivate();
		pAssetManager->CheckMaterial( m_pMaterial );
	}

	m_pMaterial = pAssetManager->GetMaterial( sMaterialPath );

	if (m_pMaterial)
		m_pMaterial->Activate();
}