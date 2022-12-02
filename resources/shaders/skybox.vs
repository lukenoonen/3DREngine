#version 330 core

#subshader CLIP

layout (location = 0) in vec3 a_vecPos;

#include "viewBuffer"
#if CLIP_TRUE
#include "clipBuffer"
#endif // CLIP_TRUE

out vec3 v_vecTexCoords;

void main()
{
	v_vecTexCoords = a_vecPos.xzy;
	vec4 vecPos = u_matProjectionViewLocked * vec4(a_vecPos, 1.0f);
	gl_Position = vecPos.xyww;
#if CLIP_TRUE
    gl_ClipDistance[0] = dot(u_matProjectionViewLocked * vec4(a_vecPos, 1.0f), u_vecClipPlane);
#endif // CLIP_TRUE
}