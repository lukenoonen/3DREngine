#version 330 core

#subshader CLIP

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

#include "shadowBuffer"

out vec4 v_vecFragPos;

void main()
{
	for(int i = 0; i < 6; i++)
	{
		gl_Layer = i;
		for(int j = 0; j < 3; j++)
		{
			v_vecFragPos = gl_in[j].gl_Position;
#if CLIP_TRUE
			gl_ClipDistance[0] = gl_in[j].gl_ClipDistance[0];
#endif // CLIP_TRUE
			gl_Position = u_matShadowMatrix[i] * v_vecFragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}