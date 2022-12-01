#ifndef TEXTURERESOURCE_H
#define TEXTURERESOURCE_H

#include "Global.h"
#include "BaseResource.h"
#include "TextureResourceData.h"

class CTextureResource : public CBaseResource
{
public:
	DECLARE_CLASS( CTextureResource, CBaseResource )

	CTextureResource( STextureResourceData &sData, const char *sName );
	virtual ~CTextureResource();

	int Bind( void ) const;

private:
	GLuint m_glID;
};

bool UTIL_Write( CFile *pFile, CTextureResource *&pData );
bool UTIL_Read( CFile *pFile, CTextureResource *&pData );
bool UTIL_GetValue( const CTextItem *pTextItem, CTextureResource *&pValue );

#endif // TEXTURERESOURCE_H