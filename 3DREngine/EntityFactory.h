#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "Global.h"

class CBaseEntity;

class CBaseEntityFactory
{
public:
	DECLARE_CLASS_NOBASE( CBaseEntityFactory )

	CBaseEntityFactory( const char *sMapName );

	virtual CBaseEntity *CreateEntity( void );

	const char *GetMapName( void );

private:
	const char *m_sMapName;
};

template <class T> class CEntityFactory : public CBaseEntityFactory
{
public:
	DECLARE_CLASS( CEntityFactory<T>, CBaseEntityFactory )

	CEntityFactory( const char *sMapName );

	virtual CBaseEntity *CreateEntity( void );
};

template <class T> CEntityFactory<T>::CEntityFactory( const char *sMapName ) : BaseClass( sMapName )
{

}

template <class T> CBaseEntity *CEntityFactory<T>::CreateEntity( void )
{
	return new T;
}

// TODO: see if GetMapName() is useful, if not, remove it
#define DECLARE_LINKED_CLASS() \
	virtual const char *GetMapName( void ) const;

#define DEFINE_LINKED_CLASS( className, mapName ) \
	static CEntityFactory<className> mapName( #mapName ); \
	const char *className::GetMapName( void ) const { return #mapName; }

#endif // ENTITYFACTORY_H