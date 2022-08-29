#version 330 core

#subtype SUBTYPE_DIR
#subtype SUBTYPE_POINT
#subtype SUBTYPE_SPOT
#subtype SUBTYPE_CSM

#if SUBTYPE_POINT || SUBTYPE_SPOT
#include "lightPositionBuffer.sh"
#include "lightMaxDistance.sh"

in vec3 v_vecFragPos;
#endif // SUBTYPE_POINT || SUBTYPE_SPOT

// Functions
void main()
{
#if SUBTYPE_POINT || SUBTYPE_SPOT
	gl_FragDepth = length(u_vecLightPosition - v_vecFragPos) / u_flLightMaxDistance;
#endif // SUBTYPE_POINT || SUBTYPE_SPOT
}