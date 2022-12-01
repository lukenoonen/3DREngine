#version 330 core

in vec2 v_vecTexCoords;

uniform sampler2D u_sDiffuse;

out vec4 v_vecFragColor;

void main()
{
    v_vecFragColor = vec4(texture(u_sDiffuse, v_vecTexCoords).xyz, 1.0);
}