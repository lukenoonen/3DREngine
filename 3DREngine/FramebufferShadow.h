#ifndef FRAMEBUFFERSHADOW_H
#define FRAMEBUFFERSHADOW_H

#include "Global.h"
#include "BaseFramebuffer.h"
#include "DataDesc.h"

class CFramebufferShadow : public CBaseFramebuffer
{
public:
	DECLARE_CLASS( CFramebufferShadow, CBaseFramebuffer )

	DECLARE_DATADESC()

	CFramebufferShadow();

	virtual int Bind( void ) const;
	virtual GLuint GetFramebuffer( void ) const;

	virtual const glm::ivec2 &GetSize( void ) const;

protected:
	virtual bool SetSizeInternal( const glm::ivec2 &vec2Size );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

protected:
	glm::ivec2 m_vec2Size;

	GLuint m_glTextureFBO;
	GLuint m_glTexture;
};

#endif // FRAMEBUFFERSHADOW_H