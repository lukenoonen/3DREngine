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

	AddFlags( FL_ANIMATE | FL_UPDATEANIMATION );

	return true;
}