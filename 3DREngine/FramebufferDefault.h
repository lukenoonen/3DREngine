#ifndef FRAMEBUFFERDEFAULT_H
#define FRAMEBUFFERDEFAULT_H

#include "Global.h"
#include "BaseFramebuffer.h"

class CFramebufferDefault : public CBaseFramebuffer
{
public:
	DECLARE_CLASS( CFramebufferDefault, CBaseFramebuffer )

	CFramebufferDefault();

	virtual void Init( void );

	virtual void Think( void );

	virtual GLuint GetFramebuffer( void ) const;

	virtual void Blit( void );

	virtual const glm::ivec2 &GetSize( void ) const;
	virtual unsigned char GetMSAALevel( void ) const;

protected:
	virtual bool SetSizeInternal( const glm::ivec2 &vec2Size );
	virtual bool SetMSAALevelInternal( unsigned char ucMSAALevel );

	virtual void CreateMSAABuffers( void );
	virtual void DestroyMSAABuffers( void );

private:
	glm::ivec2 m_vec2Size;
	unsigned char m_ucMSAALevel;

	GLuint m_glMSAAFBO;
	GLuint m_glMSAARBO;
	GLuint m_glMSAA;
};

#endif // FRAMEBUFFERDEFAULT_H