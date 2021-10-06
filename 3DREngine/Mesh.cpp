#include "Mesh.h"
#include "AssetManager.h"

CMesh::CMesh( CGeometry *pGeometry, CMaterial *pMaterial, const char *sPath ) : BaseClass( sPath )
{
	m_pGeometry = pGeometry;
	m_pMaterial = pMaterial;

	m_pGeometry->Activate();
	m_pMaterial->Activate();
}

CMesh::~CMesh()
{
	m_pGeometry->Inactivate();
	pAssetManager->CheckGeometry( m_pGeometry );

	m_pMaterial->Inactivate();
	pAssetManager->CheckMaterial( m_pMaterial );
}

bool CMesh::ShouldDraw( void ) const
{
	return m_pMaterial->ShouldDraw();
}

void CMesh::Draw( void ) 
{
	m_pMaterial->Use();
	m_pGeometry->Draw();
}