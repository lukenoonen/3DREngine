#version 330 core

#include "lightPositionBuffer.sh"
#include "lightMaxDistance.sh"

in vec3 v_vecFragPos;

// Functions
void main()
{
	gl_FragDepth = length(u_vecLightPosition - v_vecFragPos) / u_flLightMaxDistance;
}