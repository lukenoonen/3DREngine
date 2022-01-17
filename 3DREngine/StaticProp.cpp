#include "StaticProp.h"
#include "AssetManager.h"

CStaticProp::CStaticProp( const char *sModelPath )
{
	CModel *pModel = pAssetManager->GetModel( sModelPath );
	pModel->Activate();
	SetModel( pModel );
}

CStaticProp::~CStaticProp()
{
	CModel *pModel = GetModel();
	pModel->Inactivate();
	pAssetManager->CheckModel( pModel );
}