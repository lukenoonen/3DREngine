#include "BaseDrawable.h"
#include "ShaderManager.h"

CBaseDrawable::CBaseDrawable() : BaseClass()
{
	m_bShouldDraw = true;
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
	return m_bShouldDraw;
}

void CBaseDrawable::SetShouldDraw( bool bShouldDraw )
{
	m_bShouldDraw = bShouldDraw;
}