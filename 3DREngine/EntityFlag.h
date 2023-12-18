#ifndef ENTITYFLAG_H
#define ENTITYFLAG_H

#include "Global.h"

class CEntityFlag
{
public:
	DECLARE_CLASS_NOBASE( CEntityFlag )

	CEntityFlag( const char *sKey, int iFlag );

	inline const char *GetKey( void ) const { return m_sKey; }
	inline int GetFlag( void ) const { return m_iFlag; }

private:
	const char *m_sKey;
	int m_iFlag;
};

#define DEFINE_ENTITY_FLAG( key, flag ) \
	static CEntityFlag key( #key, flag );

#endif // ENTITYFLAG_H