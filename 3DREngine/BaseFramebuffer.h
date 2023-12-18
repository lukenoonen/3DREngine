#ifndef BASEFRAMEBUFFER_H
#define BASEFRAMEBUFFER_H

#include "Global.h"

class CBaseFramebuffer
{
public:
	DECLARE_CLASS_NOBASE( CBaseFramebuffer )

	CBaseFramebuffer();

	virtual int Bind( void );

	virtual const glm::ivec2 &GetSize( void ) const;
	virtual GLuint GetFramebuffer( void ) const;

	virtual void Blit( void );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

	virtual void CreateMSAABuffers( void );
	virtual void DestroyMSAABuffers( void );
};

#endif // BASEFRAMEBUFFER_H