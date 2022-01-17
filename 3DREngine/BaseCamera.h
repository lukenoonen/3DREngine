#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "Global.h"
#include "BaseTransform.h"

class CBaseCamera : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseCamera, CBaseTransform );

	CBaseCamera( unsigned int uiRenderPriority );

	virtual bool IsCamera( void ) const;

	virtual void Render( void );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

	virtual void CreateMSAABuffers( void );
	virtual void DestroyMSAABuffers( void );

	unsigned int GetRenderPriority( void ) const;

private:
	unsigned int m_uiRenderPriority;

};

/*
class CBaseCamera : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseCamera, CBaseTransform );

	CBaseCamera( const glm::ivec2 &vecSize, unsigned int uiMSAALevel, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
	virtual ~CBaseCamera();

	virtual bool IsCamera( void ) const;
	
	virtual void Render( void );

	void SetSize( const glm::ivec2 vecSize );
	void SetMSAALevel( unsigned int uiMSAALevel );

	const glm::ivec2 &GetSize( void ) const;
	unsigned int GetMSAALevel( void ) const;

	void CreateTextureBuffers( void );
	void DestroyTextureBuffers( void );

	void CreateMSAABuffers( void );
	void DestroyMSAABuffers( void );

private:
	glm::ivec2 m_vecSize;
	unsigned int m_uiMSAALevel;

	unsigned int m_uiTextureFBO;
	unsigned int m_uiTexture;

	unsigned int m_uiMSAAFBO;
	unsigned int m_uiMSAA;
	unsigned int m_uiMSAARBO;
};*/

#endif // BASECAMERA_H