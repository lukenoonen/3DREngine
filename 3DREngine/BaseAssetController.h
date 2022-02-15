#ifndef BASEASSETCONTROLLER_H
#define BASEASSETCONTROLLER_H

#include "Global.h"

class CBaseAssetController
{
public:
	DECLARE_CLASS_NOBASE( CBaseAssetController );

	CBaseAssetController( const char *sPath );
	virtual ~CBaseAssetController();

	const char *GetPath( void ) const;

	void Activate( void );
	void Inactivate( void );
	bool IsActive( void ) const;

private:

};

#endif // BASEASSETCONTROLLER_H