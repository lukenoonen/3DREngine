#version 330 core

#include "lightPositionBuffer.sh"
#include "lightMaxDistance.sh"

in vec4 v_vecFragPos;

// Functions
void main()
{
	gl_FragDepth = length(u_vecLightPosition - v_vecFragPos.xyz) / u_flLightMaxDistance;
}