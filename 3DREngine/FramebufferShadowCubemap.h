#ifndef FRAMEBUFFERSHADOWCUBEMAP_H
#define FRAMEBUFFERSHADOWCUBEMAP_H

#include "Global.h"
#include "FramebufferShadow.h"

class CFramebufferShadowCubemap : public CFramebufferShadow
{
public:
	DECLARE_CLASS( CFramebufferShadowCubemap, CFramebufferShadow )

	CFramebufferShadowCubemap();

	virtual int Bind( void ) const;

protected:
	virtual void CreateTextureBuffers( void );
};

#endif // FRAMEBUFFERSHADOWCUBEMAP_H