#version 330

layout (location = 0) in vec2 a_vecPos;
layout (location = 1) in vec2 a_vecTexCoords;

#include "viewBuffer"
#include "modelBuffer"

uniform vec2 u_vecOffset;
uniform float u_flScale;

out vec2 v_vecTexCoords;

void main(void)
{
	gl_Position = u_matProjectionView * u_matModel * (vec4(a_vecPos.x * u_flScale + u_vecOffset.x, 0.0f, a_vecPos.y * u_flScale - u_vecOffset.y, 1.0f));
	v_vecTexCoords = a_vecTexCoords;
}