#version 330 core

layout (location = 0) in vec3 a_vecPos;

#include "viewBuffer.sh"

out vec3 v_vecTexCoords;

void main()
{
	v_vecTexCoords = a_vecPos.xzy;
	vec4 vecPos = u_matProjectionViewLocked * vec4(a_vecPos, 1.0);
	gl_Position = vecPos.xyww;
}