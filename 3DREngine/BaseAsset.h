#ifndef BASEASSET_H
#define BASEASSET_H

#include "Global.h"

class CBaseAsset
{
public:
	DECLARE_CLASS_NOBASE( CBaseAsset );

	CBaseAsset( const char *sPath );
	virtual ~CBaseAsset();

	const char *GetPath( void ) const;

	void Activate( void );
	void Inactivate( void );
	bool IsActive( void ) const;

private:
	char *m_sPath;
	unsigned int m_uiActive;
};

#endif // BASEASSET_H