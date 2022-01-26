#ifndef BASEDRAWABLE_H
#define BASEDRAWABLE_H

#include "Global.h"
#include "BaseEntity.h"

class CBaseDrawable : public CBaseEntity
{
public:
	DECLARE_CLASS( CBaseDrawable, CBaseEntity );

	CBaseDrawable();
	
	virtual void PreDraw( void );
	virtual void Draw( void );
	virtual void PostDraw( void );

	virtual bool IsDrawable( void ) const;

	virtual bool ShouldDraw( void ) const;
	void SetShouldDraw( bool bShouldDraw );

	void SetDrawIndex( unsigned int uiDrawIndex );
	unsigned int GetDrawIndex( void ) const;

private:
	bool m_bShouldDraw;

	unsigned int m_uiDrawIndex;
};

#endif // BASEDRAWABLE_H