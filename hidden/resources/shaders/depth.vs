#version 330 core

#subshader ANIMATE
#subshader CLIP

layout (location = 0) in vec3 a_vecPos;
layout (location = 5) in ivec4 a_vecBoneIDs;
layout (location = 6) in vec4 a_vecWeights;

#include "viewBuffer.sh"
#include "modelBuffer.sh"
#include "bonesBuffer.sh"
#if CLIP_TRUE
#include "clipBuffer.sh"
out float gl_ClipDistance[1];
#endif // CLIP_TRUE

void main()
{
#if ANIMATE_TRUE
	mat4 matBoneTransform = u_matBones[a_vecBoneIDs.w] * a_vecWeights.w + u_matBones[a_vecBoneIDs.x] * a_vecWeights.x + u_matBones[a_vecBoneIDs.y] * a_vecWeights.y + u_matBones[a_vecBoneIDs.z] * a_vecWeights.z;
	gl_Position = u_matProjectionView * u_matModel * matBoneTransform * vec4(a_vecPos, 1.0);
#if CLIP_TRUE
    gl_ClipDistance[0] = dot(u_matModel * matBoneTransform * vec4(a_vecPos, 1.0), u_vecClipPlane);
#endif // CLIP_TRUE
#else // ANIMATE_TRUE
	gl_Position = u_matProjectionView * u_matModel * vec4(a_vecPos, 1.0);
#if CLIP_TRUE
    gl_ClipDistance[0] = dot(u_matModel * vec4(a_vecPos, 1.0), u_vecClipPlane);
#endif // CLIP_TRUE
#endif // ANIMATE_TRUE
}