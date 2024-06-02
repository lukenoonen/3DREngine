#version 330

layout (location = 0) in vec2 a_vecPos;
layout (location = 1) in vec2 a_vecTexCoords;

#include "viewBuffer"
#include "modelBuffer"

out vec2 v_vecTexCoords;

void main(void)
{
	gl_Position = vec4(a_vecPos.x, a_vecPos.y, 1.0f, 1.0f); 
	v_vecTexCoords = a_vecTexCoords;
}