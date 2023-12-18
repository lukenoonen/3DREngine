#ifndef FRAMEBUFFERSHADOWCUBEMAP_H
#define FRAMEBUFFERSHADOWCUBEMAP_H

#include "Global.h"
#include "FramebufferShadow.h"

class CFramebufferShadowCubemap : public CFramebufferShadow
{
public:
	DECLARE_CLASS( CFramebufferShadowCubemap, CFramebufferShadow )

	CFramebufferShadowCubemap();

	virtual void CreateTextureBuffers( void );
};

#endif // FRAMEBUFFERSHADOWCUBEMAP_H