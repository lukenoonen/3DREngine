#version 330

in vec2 v_vecTexCoords;

out vec4 v_vecColour;

uniform vec3 u_vecColor;

void main(void)
{
	v_vecColour = vec4(u_vecColor, 1.0f);
}