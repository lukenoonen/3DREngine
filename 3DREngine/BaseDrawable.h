#ifndef BASEDRAWABLE_H
#define BASEDRAWABLE_H

#include "Global.h"
#include "BaseEntity.h"

DEFINE_ENTITY_FLAG( fl_draw, 0 )

DEFINE_ENTITY_FLAG( dfl_menu, 0 )
DEFINE_ENTITY_FLAG( dfl_hud, 1 )
DEFINE_ENTITY_FLAG( dfl_0, 2 )
DEFINE_ENTITY_FLAG( dfl_1, 3 )
DEFINE_ENTITY_FLAG( dfl_2, 4 )
DEFINE_ENTITY_FLAG( dfl_3, 5 )
DEFINE_ENTITY_FLAG( dfl_4, 6 )
DEFINE_ENTITY_FLAG( dfl_5, 7 )
DEFINE_ENTITY_FLAG( dfl_6, 8 )
DEFINE_ENTITY_FLAG( dfl_7, 9 )
DEFINE_ENTITY_FLAG( dfl_8, 10 )
DEFINE_ENTITY_FLAG( dfl_9, 11 )
DEFINE_ENTITY_FLAG( dfl_10, 12 )
DEFINE_ENTITY_FLAG( dfl_11, 13 )
DEFINE_ENTITY_FLAG( dfl_12, 14 )
DEFINE_ENTITY_FLAG( dfl_13, 15 )
DEFINE_ENTITY_FLAG( dfl_14, 16 )
DEFINE_ENTITY_FLAG( dfl_15, 17 )
DEFINE_ENTITY_FLAG( dfl_16, 18 )
DEFINE_ENTITY_FLAG( dfl_17, 19 )
DEFINE_ENTITY_FLAG( dfl_18, 20 )
DEFINE_ENTITY_FLAG( dfl_19, 21 )
DEFINE_ENTITY_FLAG( dfl_20, 22 )
DEFINE_ENTITY_FLAG( dfl_21, 23 )
DEFINE_ENTITY_FLAG( dfl_22, 24 )
DEFINE_ENTITY_FLAG( dfl_23, 25 )
DEFINE_ENTITY_FLAG( dfl_24, 26 )
DEFINE_ENTITY_FLAG( dfl_25, 27 )
DEFINE_ENTITY_FLAG( dfl_26, 28 )
DEFINE_ENTITY_FLAG( dfl_27, 29 )
DEFINE_ENTITY_FLAG( dfl_28, 30 )
DEFINE_ENTITY_FLAG( dfl_29, 31 )

class CBaseDrawable : public CBaseEntity
{
public:
	DECLARE_CLASS( CBaseDrawable, CBaseEntity )

	DECLARE_DATADESC()

	CBaseDrawable();
	
	virtual void PreRender( void );

	virtual void PreDraw( void );
	virtual void Draw( void );
	virtual void PostDraw( void );

	virtual bool IsDrawable( void ) const;

	virtual bool ShouldDraw( void ) const;

	bool HasDrawFlags( int iDrawFlags ) const;
	void AddDrawFlags( int iDrawFlags );
	void RemoveDrawFlags( int iDrawFlags );

	int GetDrawFlags( void ) const;

	bool IsFirstPass( void ) const;

private:
	int m_iDrawFlags;
	bool m_bFirstPass;
};

#endif // BASEDRAWABLE_H