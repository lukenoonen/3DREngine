#ifndef BASESIZECAMERA_H
#define BASESIZECAMERA_H

#include "Global.h"
#include "BaseCamera.h"

class CBaseSizeCamera : public CBaseCamera
{
public:
	DECLARE_CLASS( CBaseSizeCamera, CBaseCamera );

	CBaseSizeCamera();

	void SetSize( const glm::ivec2 vecSize );

protected:
	const glm::ivec2 &GetSize( void ) const;

private:
	glm::ivec2 m_vecSize;
};

#endif // BASESIZECAMERA_H