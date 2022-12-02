#version 330 core

#subshader ANIMATE
#subshader CLIP
#subshader REFLECTION

layout (location = 0) in vec3 a_vecPos;
layout (location = 1) in vec3 a_vecNormal;
layout (location = 2) in vec3 a_vecTangent;
layout (location = 3) in vec3 a_vecBitangent;
layout (location = 4) in vec2 a_vecTexCoords;
layout (location = 5) in ivec4 a_vecBoneIDs;
layout (location = 6) in vec4 a_vecWeights;

#include "viewBuffer"
#include "modelBuffer"
#include "bonesBuffer"
#if CLIP_TRUE
#include "clipBuffer"
#endif // CLIP_TRUE
#if REFLECTION_TRUE
#include "reflectionBuffer"
#endif // REFLECTION_TRUE

uniform vec2 u_vecTextureScale;

out vec2 v_vecTexCoords;
#if REFLECTION_TRUE
out float v_flReflectionMapFactor;
out vec2 v_vecReflectionMapCoords;
#endif // REFLECTION_TRUE

void main()
{
	v_vecTexCoords = a_vecTexCoords * u_vecTextureScale;
	
#if ANIMATE_TRUE
	mat4 matBoneTransform = u_matBones[a_vecBoneIDs.w] * a_vecWeights.w + u_matBones[a_vecBoneIDs.x] * a_vecWeights.x + u_matBones[a_vecBoneIDs.y] * a_vecWeights.y + u_matBones[a_vecBoneIDs.z] * a_vecWeights.z;
	gl_Position = u_matProjectionView * u_matModel * matBoneTransform * vec4(a_vecPos, 1.0);
#if CLIP_TRUE
    gl_ClipDistance[0] = dot(u_matModel * matBoneTransform * vec4(a_vecPos, 1.0), u_vecClipPlane);
#endif // CLIP_TRUE
#if REFLECTION_TRUE
	vec4 vecFragPosReflectionSpace = u_matReflectionMatrix * vec4((u_matModel * matBoneTransform * vec4(a_vecPos, 1.0)).xyz, 1.0f);
	v_flReflectionMapFactor = vecFragPosReflectionSpace.w;
	v_vecReflectionMapCoords = vecFragPosReflectionSpace.xy;
#endif // REFLECTION_TRUE
#else // ANIMATE_TRUE
	gl_Position = u_matProjectionView * u_matModel * vec4(a_vecPos, 1.0);
#if CLIP_TRUE
    gl_ClipDistance[0] = dot(u_matModel * vec4(a_vecPos, 1.0), u_vecClipPlane);
#endif // CLIP_TRUE
#if REFLECTION_TRUE
	vec4 vecFragPosReflectionSpace = u_matReflectionMatrix * vec4((u_matModel * vec4(a_vecPos, 1.0)).xyz, 1.0f);
	v_flReflectionMapFactor = vecFragPosReflectionSpace.w;
	v_vecReflectionMapCoords = vecFragPosReflectionSpace.xy;
#endif // REFLECTION_TRUE
#endif // ANIMATE_TRUE
}