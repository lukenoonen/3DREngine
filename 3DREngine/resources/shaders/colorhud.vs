#version 330 core

layout (location = 0) in vec2 a_vecPos;
layout (location = 1) in vec2 a_vecTexCoords;

#include "HUDBuffer.sh"

void main()
{
	gl_Position = vec4(a_vecPos.xy * u_vecHUDSize + u_vecHUDPosition, u_flHUDPriority, 1.0f);
}