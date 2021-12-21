#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

#include "shadowBuffer.sh"

out vec4 v_vecFragPos;

void main()
{
	for(int i = 0; i < 6; i++)
	{
		gl_Layer = i;
		for(int j = 0; j < 3; j++)
		{
			v_vecFragPos = gl_in[j].gl_Position;
			gl_Position = u_matShadowMatrix[i] * v_vecFragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}