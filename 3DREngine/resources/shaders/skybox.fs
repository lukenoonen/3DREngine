#version 330 core

in vec3 v_vecTexCoords;

uniform samplerCube u_sSkybox;

out vec4 v_vecFragColor;

void main()
{    
	v_vecFragColor = texture(u_sSkybox, v_vecTexCoords);
}