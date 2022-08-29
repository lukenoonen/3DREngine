#version 330 core

#subshader ANIMATE
#subshader SHADOW

layout (location = 0) in vec3 a_vecPos;
layout (location = 1) in vec3 a_vecNormal;
layout (location = 2) in vec3 a_vecTangent;
layout (location = 3) in vec3 a_vecBitangent;
layout (location = 4) in vec2 a_vecTexCoords;
layout (location = 5) in ivec4 a_vecBoneIDs;
layout (location = 6) in vec4 a_vecWeights;

#include "viewBuffer.sh"
#include "modelBuffer.sh"
#include "bonesBuffer.sh"
#include "lightDirectionBuffer.sh"
#if SHADOW_TRUE
#include "shadowBuffer.sh"
#endif // SHADOW_TRUE

uniform vec2 u_vecTextureScale;

out vec2 v_vecTexCoords;
out float v_flClipSpacePosZ;
out vec3 v_vecFragPos;
out vec3 v_vecNormal;
out vec3 v_vecTangentLightPos;
out vec3 v_vecTangentViewPos;
out vec3 v_vecTangentFragPos;
#if SHADOW_TRUE
out float v_flShadowMapFactor[4];
out vec2 v_vecShadowMapCoords[4];
out float v_flShadowMapDepth[4];
#endif // SHADOW_TRUE

void main()
{
	v_vecTexCoords = a_vecTexCoords * u_vecTextureScale;
	
#if ANIMATE_TRUE
	mat4 matBoneTransform = u_matBones[a_vecBoneIDs.w] * a_vecWeights.w + u_matBones[a_vecBoneIDs.x] * a_vecWeights.x + u_matBones[a_vecBoneIDs.y] * a_vecWeights.y + u_matBones[a_vecBoneIDs.z] * a_vecWeights.z;
	gl_Position = u_matProjectionView * u_matModel * matBoneTransform * vec4(a_vecPos, 1.0);
	v_vecFragPos = (u_matModel * matBoneTransform * vec4(a_vecPos, 1.0)).xyz;
	v_flClipSpacePosZ = gl_Position.z;
	v_vecNormal = normalize(vec3(u_matTIModel * matBoneTransform * vec4(a_vecNormal, 0.0f)));
	mat3 matTBN = transpose(mat3(normalize(vec3(u_matTIModel * matBoneTransform * vec4(a_vecTangent, 0.0f))), normalize(vec3(u_matTIModel * matBoneTransform * vec4(a_vecBitangent, 0.0f))), v_vecNormal));
#else // ANIMATE_TRUE
	gl_Position = u_matProjectionView * u_matModel * vec4(a_vecPos, 1.0);
	v_vecFragPos = (u_matModel * vec4(a_vecPos, 1.0)).xyz;
	v_flClipSpacePosZ = gl_Position.z;
	v_vecNormal = normalize(vec3(u_matTIModel * vec4(a_vecNormal, 0.0f)));
	mat3 matTBN = transpose(mat3(normalize(vec3(u_matTIModel * vec4(a_vecTangent, 0.0f))), normalize(vec3(u_matTIModel * vec4(a_vecBitangent, 0.0f))), v_vecNormal));
#endif // ANIMATE_TRUE
	
	v_vecTangentLightPos = matTBN * (v_vecFragPos - u_vecLightDirection);
	v_vecTangentViewPos = matTBN * u_vecViewPos;
	v_vecTangentFragPos = matTBN * v_vecFragPos;
	
#if SHADOW_TRUE
	vec4 vecFragPosLightSpace = u_matShadowMatrix[0] * vec4(v_vecFragPos, 1.0f);
	v_flShadowMapFactor[0] = vecFragPosLightSpace.w;
	v_vecShadowMapCoords[0] = vecFragPosLightSpace.xy;
	v_flShadowMapDepth[0] = vecFragPosLightSpace.z;
	vecFragPosLightSpace = u_matShadowMatrix[1] * vec4(v_vecFragPos, 1.0f);
	v_flShadowMapFactor[1] = vecFragPosLightSpace.w;
	v_vecShadowMapCoords[1] = vecFragPosLightSpace.xy;
	v_flShadowMapDepth[1] = vecFragPosLightSpace.z;
	vecFragPosLightSpace = u_matShadowMatrix[2] * vec4(v_vecFragPos, 1.0f);
	v_flShadowMapFactor[2] = vecFragPosLightSpace.w;
	v_vecShadowMapCoords[2] = vecFragPosLightSpace.xy;
	v_flShadowMapDepth[2] = vecFragPosLightSpace.z;
	vecFragPosLightSpace = u_matShadowMatrix[3] * vec4(v_vecFragPos, 1.0f);
	v_flShadowMapFactor[3] = vecFragPosLightSpace.w;
	v_vecShadowMapCoords[3] = vecFragPosLightSpace.xy;
	v_flShadowMapDepth[3] = vecFragPosLightSpace.z;
#endif // SHADOW_TRUE
}