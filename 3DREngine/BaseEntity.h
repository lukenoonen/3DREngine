#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "Global.h"

class CBaseEntity
{
public:
	DECLARE_CLASS_NOBASE( CBaseEntity );

	CBaseEntity( bool bActive );
	
	virtual void PreThink( void );
	virtual void Think( void );
	virtual void PostThink( void );

	virtual bool IsPlayer( void ) const;
	virtual bool IsLight( void ) const;
	virtual bool IsDrawable( void ) const;
	virtual bool IsCamera( void ) const;

	bool IsActive( void ) const;
	void SetActive( bool bActive );

private:
	bool m_bActive;
};

#endif // BASEENTITY_H