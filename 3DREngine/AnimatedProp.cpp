#include "AnimatedProp.h"
#include "AssetManager.h"

CAnimatedProp::CAnimatedProp()
{

}

void CAnimatedProp::Init( void )
{
	if (GetModel())
	{
		SetAnimation( 0 );

		SetUpdateAnimation( true );
		SetAnimate( true );
	}

	BaseClass::Init();
}

void CAnimatedProp::Exit( void )
{
	CModel *pModel = GetModel();
	pModel->Inactivate();
	pAssetManager->CheckModel( pModel );

	BaseClass::Exit();
}

void CAnimatedProp::SetModel( const char *sModelPath )
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