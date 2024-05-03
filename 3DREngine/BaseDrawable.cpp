#include "BaseDrawable.h"
#include "EntityManager.h"
#include "BaseCamera.h"

DEFINE_DATADESC( CBaseDrawable )

	DEFINE_FIELD( FlagDataField, int, m_iDrawFlags, "drawflags", FL_NONE )

END_DATADESC()

CBaseDrawable::CBaseDrawable()
{
	AddFlags( fl_draw.GetFlag() );

	m_iDrawFlags = 0;
}

void CBaseDrawable::PreRender( void )
{

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
	CBaseCamera *pCurrentCamera = pEntityManager->GetCurrentCamera();
	return HasFlags( fl_draw.GetFlag() ) && pCurrentCamera && pCurrentCamera->HasDrawFlags( m_iDrawFlags );
}

bool CBaseDrawable::HasDrawFlags( int iDrawFlags ) const
{
	return (m_iDrawFlags & iDrawFlags) != 0;
}

void CBaseDrawable::AddDrawFlags( int iDrawFlags )
{
	m_iDrawFlags |= iDrawFlags;
}

void CBaseDrawable::RemoveDrawFlags( int iDrawFlags )
{
	m_iDrawFlags &= ~iDrawFlags;
}