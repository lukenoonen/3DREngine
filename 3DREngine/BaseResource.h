#ifndef BASERESOURCE_H
#define BASERESOURCE_H

#include "Global.h"

// TODO: make asset container classes for the resources

class CBaseResource
{
public:
	DECLARE_CLASS_NOBASE( CBaseResource )

	CBaseResource( const char *sName );
	virtual ~CBaseResource();

	const char *GetName( void ) const;

	void Reference( void );
	void Unreference( void );
	bool IsReferenced( void ) const;

private:
	char *m_sName;
	unsigned int m_uiReferences;
};

#endif // BASERESOURCE_H