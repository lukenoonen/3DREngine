#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "Global.h"
#include "DataDesc.h"
#include "Handle.h"
#include "EntityFactory.h"

class CBaseEntity
{
public:
	DECLARE_CLASS_NOBASE( CBaseEntity )

	DECLARE_DATADESC()

	CBaseEntity();
	virtual ~CBaseEntity();

	virtual bool Init( void );
	
	virtual void PreThink( void );
	virtual void Think( void );
	virtual void PostThink( void );

	virtual bool IsPlayer( void ) const;
	virtual bool IsLight( void ) const;
	virtual bool IsCamera( void ) const;
	virtual bool IsDrawable( void ) const;

	const char *GetName( void ) const;
	const char *GetFileName( void ) const;
	virtual const char *GetMapName( void ) const;

	bool HasFlags( int iFlags ) const;
	void AddFlags( int iFlags );
	void RemoveFlags( int iFlags );

	void Remove( void );
	bool IsRemoved( void ) const;

	void Reference( void );
	void Unreference( void );
	bool IsReferenced( void ) const;

private:
	char *m_sName;
	char *m_sFileName;

	int m_iFlags;

	bool m_bRemoved;
	unsigned int m_uiReferences;
};

#endif // BASEENTITY_H