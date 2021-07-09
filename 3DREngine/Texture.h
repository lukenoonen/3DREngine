#ifndef TEXTURE_H
#define TEXTURE_H

#include "Global.h"

class CTexture
{
public:
	DECLARE_CLASS_NOBASE( CTexture );

	CTexture( unsigned int uiID, const char *sPath );
	~CTexture();

	unsigned int GetID( void );
	const char *GetPath( void );

private:
	unsigned int m_uiID;
	char *m_sPath;
};

#endif // TEXTURE_H