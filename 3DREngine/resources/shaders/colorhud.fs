#version 330 core

uniform vec3 u_vecColor;

out vec4 v_vecFragColor;

void main()
{
	v_vecFragColor = vec4(u_vecColor, 1.0f);
}