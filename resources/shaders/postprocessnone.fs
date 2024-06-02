#version 330

in vec2 v_vecTexCoords;

out vec4 v_vecColor;

uniform sampler2D u_sCameraTexture;

void main(void)
{
	v_vecColor = texture(u_sCameraTexture, v_vecTexCoords);
}