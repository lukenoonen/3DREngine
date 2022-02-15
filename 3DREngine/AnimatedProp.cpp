#include "AnimatedProp.h"
#include "AssetManager.h"

CAnimatedProp::CAnimatedProp()
{

}

void CAnimatedProp::Init( void )
{
	SetAnimation( 0 );

	SetUpdateAnimation( true );
	SetAnimate( true );

	BaseClass::Init();
}