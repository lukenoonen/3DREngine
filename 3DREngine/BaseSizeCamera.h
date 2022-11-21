#ifndef BASESIZECAMERA_H
#define BASESIZECAMERA_H

#include "Global.h"
#include "BaseCamera.h"

class CBaseSizeCamera : public CBaseCamera
{
public:
	DECLARE_CLASS( CBaseSizeCamera, CBaseCamera )

	DECLARE_DATADESC()

	CBaseSizeCamera();

	void SetSize( const glm::ivec2 vec2Size );

	const glm::ivec2 &GetSize( void ) const;

private:
	glm::ivec2 m_vec2Size;
};

#endif // BASESIZECAMERA_H