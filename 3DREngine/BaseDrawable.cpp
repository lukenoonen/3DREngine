#include "BaseDrawable.h"
#include "ShaderManager.h"

CBaseDrawable::CBaseDrawable( bool bShouldDraw, bool bActive ) : BaseClass( bActive )
{
	m_bShouldDraw = bShouldDraw;
}

void CBaseDrawable::PreDraw( void )
{
	pShaderManager->SetShaderAnimate( SHADERANIMATE_FALSE );
}

void CBaseDrawable::Draw( void )
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