#include "BaseDrawable.h"

CBaseDrawable::CBaseDrawable()
{
	AddFlags( FL_DRAW );
}

void CBaseDrawable::PreDraw( void )
{

}

void CBaseDrawable::Draw( void )
{

}

void CBaseDrawable::PostDraw( void )
{

}

bool CBaseDrawable::IsDrawable( void ) const
{
	return true;
}

bool CBaseDrawable::ShouldDraw( void ) const
{
	return HasFlags( FL_DRAW );
}