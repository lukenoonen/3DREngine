#include "AnimatedProp.h"

DEFINE_LINKED_CLASS( CAnimatedProp, prop_animated )

CAnimatedProp::CAnimatedProp()
{

}

bool CAnimatedProp::Init( void )
{
	if (!BaseClass::Init())
		return false;

	SetAnimation( 0 );

	AddFlags( fl_animate.GetFlag() | fl_updateanimation.GetFlag() );

	return true;
}