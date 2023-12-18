#ifndef FRAMEBUFFERSHADOWCSM_H
#define FRAMEBUFFERSHADOWCSM_H

#include "Global.h"
#include "FramebufferShadow.h"

class CFramebufferShadowCSM : public CFramebufferShadow
{
public:
	DECLARE_CLASS( CFramebufferShadowCSM, CFramebufferShadow )

	CFramebufferShadowCSM();

	virtual const glm::ivec2 &GetSize( void ) const;

	virtual void CreateTextureBuffers( void );

};

#endif // FRAMEBUFFERSHADOWCSM_H