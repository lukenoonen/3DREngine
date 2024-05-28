#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "Global.h"
#include "DataDesc.h"
#include "Handle.h"
#include "Monitored.h"
#include "EntityFactory.h"
#include "BaseReferenced.h"
#include "EntityFlag.h"

class CBaseEntity : public CBaseReferenced
{
public:
	DECLARE_CLASS( CBaseEntity, CBaseReferenced )

	DECLARE_DATADESC()

	CBaseEntity();
	virtual ~CBaseEntity();

	virtual bool Init( void );
	
	virtual void PreThink( void );
	virtual void Think( void );
	virtual void PostThink( void );

	virtual void Remove( void );
	bool IsRemoved( void ) const;

	virtual void CleanUp( void );

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

	int GetFlags( void ) const;

	CEntityLoadGroup *GetLoadGroup( void ) const;
	void SetLoadGroup( CEntityLoadGroup *pLoadGroup );

	unsigned int GetLoadIndex( void ) const;

private:
	char *m_sName;
	char *m_sFileName;

	int m_iFlags;

	bool m_bRemoved;

	CEntityLoadGroup *m_pLoadGroup;
};

#endif // BASEENTITY_H