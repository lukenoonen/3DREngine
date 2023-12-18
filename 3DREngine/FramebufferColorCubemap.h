#ifndef FRAMEBUFFERCOLORCUBEMAP_H
#define FRAMEBUFFERCOLORCUBEMAP_H

#include "Global.h"
#include "FramebufferColor.h"

class CFramebufferColorCubemap : public CFramebufferColor
{
public:
	DECLARE_CLASS( CFramebufferColorCubemap, CFramebufferColor )

	CFramebufferColorCubemap();

	virtual void CreateTextureBuffers( void );

	virtual void CreateMSAABuffers( void );
};

#endif // FRAMEBUFFERCOLORCUBEMAP_H