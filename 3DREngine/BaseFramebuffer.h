#ifndef BASEFRAMEBUFFER_H
#define BASEFRAMEBUFFER_H

#include "Global.h"

class CBaseFramebuffer
{
public:
	DECLARE_CLASS_NOBASE( CBaseFramebuffer )

	CBaseFramebuffer();
	~CBaseFramebuffer();

	// TODO: FORMALISE THE INIT/THINK THINGS
	virtual void Init( void );

	virtual void Think( void );

	virtual int Bind( void ) const;
	virtual GLuint GetFramebuffer( void ) const;

	virtual void Blit( void );

	virtual const glm::ivec2 &GetSize( void ) const;
	virtual unsigned char GetMSAALevel( void ) const;

	void SetSize( const glm::ivec2 &vec2Size );
	void SetMSAALevel( unsigned char ucMSAALevel );

	void ClearBuffer( void );

protected:
	virtual void ClearBufferInternal( void );
	virtual void AdditionalClearBufferInternal( void );

	virtual bool SetSizeInternal( const glm::ivec2 &vec2Size );
	virtual bool SetMSAALevelInternal( unsigned char ucMSAALevel );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

	virtual void CreateMSAABuffers( void );
	virtual void DestroyMSAABuffers( void );

private:
	unsigned long m_ulLastRender;
};

#endif // BASEFRAMEBUFFER_H