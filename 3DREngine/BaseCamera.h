#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "Global.h"
#include "BaseTransform.h"

class CBaseCamera : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseCamera, CBaseTransform );

	CBaseCamera();

	virtual void Init( void );
	virtual void Exit( void );

	virtual void PostThink( void );

	virtual bool IsCamera( void ) const;

	virtual void Render( void );

	void SetRenderPriority( unsigned int uiRenderPriority );
	unsigned int GetRenderPriority( void ) const;

protected:
	void SetUpdateTextureBuffers( bool bUpdateTextureBuffers );
	bool GetUpdateTextureBuffers( void ) const;

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

	void SetUpdateMSAABuffers( bool bUpdateTextureBuffers );
	bool GetUpdateMSAABuffers( void ) const;

	virtual void CreateMSAABuffers( void );
	virtual void DestroyMSAABuffers( void );

private:
	unsigned int m_uiRenderPriority;

	bool m_bUpdateTextureBuffers;
	bool m_bUpdateMSAABuffers;
};

#endif // BASECAMERA_H