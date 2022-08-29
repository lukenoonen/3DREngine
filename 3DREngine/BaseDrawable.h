#ifndef BASEDRAWABLE_H
#define BASEDRAWABLE_H

#include "Global.h"
#include "BaseEntity.h"

#define FL_DRAW	(1<<0)

class CBaseDrawable : public CBaseEntity
{
public:
	DECLARE_CLASS( CBaseDrawable, CBaseEntity )

	CBaseDrawable();
	
	virtual void PreDraw( void );
	virtual void Draw( void );
	virtual void PostDraw( void );

	virtual bool IsDrawable( void ) const;

	virtual bool ShouldDraw( void ) const;
};

#endif // BASEDRAWABLE_H