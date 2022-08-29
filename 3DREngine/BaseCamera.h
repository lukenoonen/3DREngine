#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "Global.h"
#include "BaseTransform.h"

class CBaseCamera : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseCamera, CBaseTransform )

	CBaseCamera();

	virtual bool Init( void );
	virtual bool Exit( void );

	virtual void PostThink( void );

	virtual bool IsCamera( void ) const;

	virtual void Render( void );

	virtual int BindTexture( void );

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
	bool m_bUpdateTextureBuffers;
	bool m_bUpdateMSAABuffers;
};

#endif // BASECAMERA_H