#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "Global.h"
#include "BaseTransform.h"

// TODO: clean up the use of Init() in these classes and derrived classes, since they often call the same functions as PostThink()

class CBaseCamera : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseCamera, CBaseTransform )

	CBaseCamera();
	virtual ~CBaseCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual bool IsCamera( void ) const;

	virtual void Render( void );

	virtual int BindTexture( void );

protected:
	void SetUpdateTextureBuffers( bool bUpdateTextureBuffers );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

	void SetUpdateMSAABuffers( bool bUpdateTextureBuffers );

	virtual void CreateMSAABuffers( void );
	virtual void DestroyMSAABuffers( void );

private:
	bool m_bUpdateTextureBuffers;
	bool m_bUpdateMSAABuffers;
};

#endif // BASECAMERA_H