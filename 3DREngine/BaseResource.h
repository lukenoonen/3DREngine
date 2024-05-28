#ifndef BASERESOURCE_H
#define BASERESOURCE_H

#include "Global.h"
#include "BaseReferenced.h"

// TODO: finish referenced implementation

class CBaseResource : public CBaseReferenced
{
public:
	DECLARE_CLASS( CBaseResource, CBaseReferenced )

	CBaseResource( const char *sName );
	virtual ~CBaseResource();

	const char *GetName( void ) const;

private:
	char *m_sName;
};

#endif // BASERESOURCE_H