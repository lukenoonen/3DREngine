#ifndef BASEDRAWABLE_H
#define BASEDRAWABLE_H

#include "Global.h"
#include "BaseEntity.h"

class CBaseDrawable : public CBaseEntity
{
public:
	DECLARE_CLASS( CBaseDrawable, CBaseEntity );

	CBaseDrawable( bool bShouldDraw, bool bActive );
	
	virtual void PreDraw( void );
	virtual void Draw( void );
	virtual void PostDraw( void );

	virtual bool IsDrawable( void ) const;

	virtual bool ShouldDraw( void ) const;
	void SetShouldDraw( bool bShouldDraw );

private:
	bool m_bShouldDraw;
};

#endif // BASEDRAWABLE_H