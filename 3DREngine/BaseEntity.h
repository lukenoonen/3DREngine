#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "Global.h"

class CBaseEntity
{
public:
	DECLARE_CLASS_NOBASE( CBaseEntity );

	CBaseEntity();

	virtual void Init( void );
	
	virtual void PreThink( void );
	virtual void Think( void );
	virtual void PostThink( void );

	virtual void Exit( void );

	virtual bool IsPlayer( void ) const;
	virtual bool IsLight( void ) const;
	virtual bool IsDrawable( void ) const;
	virtual bool IsCamera( void ) const;

	bool IsActive( void ) const;
	void SetActive( bool bActive );

	unsigned int GetEntityIndex( void ) const;
	void SetEntityIndex( unsigned int uiEntityIndex );

private:
	bool m_bActive;

	unsigned int m_uiEntityIndex;
};

#endif // BASEENTITY_H