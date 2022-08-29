#ifndef CUBEMAPTEXTURE_H
#define CUBEMAPTEXTURE_H

#include "Global.h"
#include "BaseTexture.h"

class CCubemapTexture : public CBaseTexture
{
public:
	DECLARE_CLASS( CCubemapTexture, CBaseTexture )

	CCubemapTexture();

	virtual bool Init( void );
	virtual bool Exit( void );

	virtual int Bind( void );

private:
	GLuint m_glID;
};

#endif // CUBEMAPTEXTURE_H