#version 330

in vec2 v_vecTexCoords;

out vec4 v_vecColour;

uniform sampler2D u_sFont;
uniform vec3 u_vecColor;

void main(void)
{
	v_vecColour = vec4(u_vecColor, texture(u_sFont, v_vecTexCoords).r);
}