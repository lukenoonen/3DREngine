#include "AnimatedProp.h"
#include "AssetManager.h"

CAnimatedProp::CAnimatedProp( const char *sModelPath )
{
	CModel *pModel = pAssetManager->GetModel( sModelPath );
	pModel->Activate();
	SetModel( pModel );

	SetAnimation( 0 );

	SetUpdateAnimation( true );
	SetAnimate( true );
}

CAnimatedProp::~CAnimatedProp()
{
	CModel *pModel = GetModel();
	pModel->Inactivate();
	pAssetManager->CheckModel( pModel );
}