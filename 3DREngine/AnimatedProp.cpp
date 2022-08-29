#include "AnimatedProp.h"

CAnimatedProp::CAnimatedProp()
{

}

bool CAnimatedProp::Init( void )
{
	if (!BaseClass::Init())
		return false;

	SetAnimation( 0 );

	SetUpdateAnimation( true );
	SetAnimate( true );

	return true;
}