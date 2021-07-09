#include "Math.h"

void UTIL_RotationToDirection( const glm::vec3 &vecRotation, glm::vec3 &vecDirection )
{
	vecDirection.x = -cosf( vecRotation.z ) * sinf( vecRotation.y ) * sinf( vecRotation.x ) - sinf( vecRotation.z ) * cosf( vecRotation.x );
	vecDirection.y = -sinf( vecRotation.z ) * sinf( vecRotation.y ) * sinf( vecRotation.x ) + cosf( vecRotation.z ) * cosf( vecRotation.x );
	vecDirection.z = cosf( vecRotation.y ) * sinf( vecRotation.x );
}