#ifndef FRAMEBUFFERCOLOR_H
#define FRAMEBUFFERCOLOR_H

#include "Global.h"
#include "BaseFramebuffer.h"
#include "DataDesc.h"

class CFramebufferColor : public CBaseFramebuffer
{
public:
	DECLARE_CLASS( CFramebufferColor, CBaseFramebuffer )

	DECLARE_DATADESC()

	CFramebufferColor();

	virtual int Bind( void ) const;
	virtual GLuint GetFramebuffer( void ) const;

	virtual void Blit( void );

	virtual const glm::ivec2 &GetSize( void ) const;
	virtual unsigned char GetMSAALevel( void ) const;

protected:
	virtual void ClearBufferInternal( void );

	virtual bool SetSizeInternal( const glm::ivec2 &vec2Size );
	virtual bool SetMSAALevelInternal( unsigned char ucMSAALevel );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

	virtual void CreateMSAABuffers( void );
	virtual void DestroyMSAABuffers( void );

protected:
	glm::ivec2 m_vec2Size;
	unsigned char m_ucMSAALevel;

	GLuint m_glTextureFBO;
	GLuint m_glTextureRBO;
	GLuint m_glTexture;

	GLuint m_glMSAAFBO;
	GLuint m_glMSAARBO;
	GLuint m_glMSAA;
};

#endif // FRAMEBUFFERCOLOR_H