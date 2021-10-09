#include "AnimatedProp.h"
#include "AssetManager.h"

CAnimatedProp::CAnimatedProp( const char *sModelPath, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
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