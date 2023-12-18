#version 330

layout (location = 0) in vec2 a_vecPos;
layout (location = 1) in vec2 a_vecTexCoords;

#include "modelBuffer"

out vec2 v_vecTexCoords;

uniform vec2 u_vecTranslation;

void main(void)
{
	gl_Position = u_matModel * vec4(a_vecPos, -1.0f, 1.0f);
	v_vecTexCoords = a_vecTexCoords;
}