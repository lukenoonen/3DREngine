#ifndef CUBEMAPRESOURCE_H
#define CUBEMAPRESOURCE_H

#include "Global.h"
#include "BaseResource.h"

class CCubemapResource : public CBaseResource
{
public:
	DECLARE_CLASS( CCubemapResource, CBaseResource )
	
	CCubemapResource( unsigned char **pData, GLsizei *glWidth, GLsizei *glHeight, GLenum *tInternalFormat, GLenum *tFormat, bool bFiltered, const char *sName );
	virtual ~CCubemapResource();

	int Bind( void ) const;

private:
	GLuint m_glID;
};

bool UTIL_Write( CFile *pFile, CCubemapResource *&pData );
bool UTIL_Read( CFile *pFile, CCubemapResource *&pData );
bool UTIL_GetValue( const CTextItem *pTextItem, CCubemapResource *&pValue );

#endif // CUBEMAPRESOURCE_H