#version 330 core

#subshader ANIMATE
#subshader CLIP
#subshader SHADOW
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
#include "lightDirectionBuffer"
#if SHADOW_TRUE
#include "shadowBuffer"
#endif // SHADOW_TRUE
#if CLIP_TRUE
#include "clipBuffer"
#endif // CLIP_TRUE
#if REFLECTION_TRUE
#include "reflectionBuffer"
#endif // REFLECTION_TRUE

uniform vec2 u_vecTextureScale;

out vec2 v_vecTexCoords;
out float v_flClipSpacePosZ;
out vec3 v_vecFragPos;
out vec3 v_vecNormal;
out vec3 v_vecTangentLightPos;
out vec3 v_vecTangentViewPos;
out vec3 v_vecTangentFragPos;
#if SHADOW_TRUE
out float v_flShadowMapFactor;
out vec2 v_vecShadowMapCoords;
out float v_flShadowMapDepth;
#endif // SHADOW_TRUE
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
	v_vecFragPos = (u_matModel * matBoneTransform * vec4(a_vecPos, 1.0)).xyz;
	v_vecNormal = normalize(vec3(u_matTIModel * matBoneTransform * vec4(a_vecNormal, 0.0f)));
	mat3 matTBN = transpose(mat3(normalize(vec3(u_matTIModel * matBoneTransform * vec4(a_vecTangent, 0.0f))), normalize(vec3(u_matTIModel * matBoneTransform * vec4(a_vecBitangent, 0.0f))), v_vecNormal));

#if CLIP_TRUE
    gl_ClipDistance[0] = dot(u_matModel * matBoneTransform * vec4(a_vecPos, 1.0), u_vecClipPlane);
#endif // CLIP_TRUE

#else // ANIMATE_TRUE
	gl_Position = u_matProjectionView * u_matModel * vec4(a_vecPos, 1.0);
	v_vecFragPos = (u_matModel * vec4(a_vecPos, 1.0)).xyz;
	v_vecNormal = normalize(vec3(u_matTIModel * vec4(a_vecNormal, 0.0f)));
	mat3 matTBN = transpose(mat3(normalize(vec3(u_matTIModel * vec4(a_vecTangent, 0.0f))), normalize(vec3(u_matTIModel * vec4(a_vecBitangent, 0.0f))), v_vecNormal));

#if CLIP_TRUE
    gl_ClipDistance[0] = dot(u_matModel * vec4(a_vecPos, 1.0), u_vecClipPlane);
#endif // CLIP_TRUE

#endif // ANIMATE_TRUE

	v_flClipSpacePosZ = gl_Position.z;
	
	v_vecTangentLightPos = matTBN * (v_vecFragPos - u_vecLightDirection);
	v_vecTangentViewPos = matTBN * u_vecViewPos;
	v_vecTangentFragPos = matTBN * v_vecFragPos;
	
#if REFLECTION_TRUE
	vec4 vecFragPosReflectionSpace = u_matReflectionMatrix * vec4(v_vecFragPos, 1.0f);
	v_flReflectionMapFactor = vecFragPosReflectionSpace.w;
	v_vecReflectionMapCoords = vecFragPosReflectionSpace.xy;
#endif // REFLECTION_TRUE
	
#if SHADOW_TRUE
	vec4 vecFragPosLightSpace = u_matShadowMatrix[0] * vec4(v_vecFragPos, 1.0f);
	v_flShadowMapFactor = vecFragPosLightSpace.w;
	v_vecShadowMapCoords = vecFragPosLightSpace.xy;
	v_flShadowMapDepth = vecFragPosLightSpace.z;
#endif // SHADOW_TRUE
}
