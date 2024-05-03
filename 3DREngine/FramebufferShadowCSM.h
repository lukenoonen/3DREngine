#ifndef FRAMEBUFFERSHADOWCSM_H
#define FRAMEBUFFERSHADOWCSM_H

#include "Global.h"
#include "FramebufferShadow.h"

class CFramebufferShadowCSM : public CFramebufferShadow
{
public:
	DECLARE_CLASS( CFramebufferShadowCSM, CFramebufferShadow )

	CFramebufferShadowCSM();

	virtual void CreateTextureBuffers( void );

};

#endif // FRAMEBUFFERSHADOWCSM_H