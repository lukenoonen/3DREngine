#include "StaticProp.h"
#include "AssetManager.h"

CStaticProp::CStaticProp()
{

}

void CStaticProp::Exit( void )
{
	CModel *pModel = GetModel();
	pModel->Inactivate();
	pAssetManager->CheckModel( pModel );

	BaseClass::Exit();
}

void CStaticProp::SetModel( const char *sModelPath )
{
	CModel *pPrevModel = GetModel();
	if (pPrevModel)
	{
		pPrevModel->Inactivate();
		pAssetManager->CheckModel( pPrevModel );
	}

	CModel *pModel = pAssetManager->GetModel( sModelPath );
	if (pModel)
		pModel->Activate();

	BaseClass::SetModel( pModel );
}