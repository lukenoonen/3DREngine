#version 330 core

#subtype SUBTYPE_DIR
#subtype SUBTYPE_POINT
#subtype SUBTYPE_SPOT
#subtype SUBTYPE_CSM

#quality QUALITY_LOW
#quality QUALITY_MEDIUM
#quality QUALITY_HIGH
#quality QUALITY_ULTRA

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
#if SUBTYPE_DIR || SUBTYPE_CSM
#include "lightDirectionBuffer.sh"
#else // SUBTYPE_DIR || SUBTYPE_CSM
#include "lightPositionBuffer.sh"
#endif // SUBTYPE_DIR || SUBTYPE_CSM
#if !SUBTYPE_POINT
#include "shadowBuffer.sh"
#endif // !SUBTYPE_POINT

uniform vec2 u_vecTextureScale;

out vec2 v_vecTexCoords;
out float v_flClipSpacePosZ;
out vec3 v_vecFragPos;
out vec3 v_vecNormal;
out vec3 v_vecTangentLightPos;
out vec3 v_vecTangentViewPos;
out vec3 v_vecTangentFragPos;
#if !SUBTYPE_POINT
#if SUBTYPE_CSM
out float v_flShadowMapFactor[4];
out vec2 v_vecShadowMapCoords[4];
out float v_flShadowMapDepth[4];
#else // SUBTYPE_CSM
out float v_flShadowMapFactor;
out vec2 v_vecShadowMapCoords;
#if SUBTYPE_DIR
out float v_flShadowMapDepth;
#endif // SUBTYPE_DIR
#endif // SUBTYPE_CSM
#endif // !SUBTYPE_POINT

void main()
{
	v_vecTexCoords = a_vecTexCoords * u_vecTextureScale;
	
	mat4 matBoneTransform = (a_vecWeights.x != 0.0) ? (u_matBones[a_vecBoneIDs.w] * a_vecWeights.w + u_matBones[a_vecBoneIDs.x] * a_vecWeights.x + u_matBones[a_vecBoneIDs.y] * a_vecWeights.y + u_matBones[a_vecBoneIDs.z] * a_vecWeights.z) : mat4(1.0);
	gl_Position = u_matProjectionView * u_matModel * matBoneTransform * vec4(a_vecPos, 1.0);
	v_vecFragPos = (u_matModel * matBoneTransform * vec4(a_vecPos, 1.0)).xyz;
	v_flClipSpacePosZ = gl_Position.z;
	
	v_vecNormal = normalize(vec3(u_matTIModel * matBoneTransform * vec4(a_vecNormal, 0.0f)));
	mat3 matTBN = transpose(mat3(normalize(vec3(u_matTIModel * matBoneTransform * vec4(a_vecTangent, 0.0f))), normalize(vec3(u_matTIModel * matBoneTransform * vec4(a_vecBitangent, 0.0f))), v_vecNormal));
#if SUBTYPE_DIR || SUBTYPE_CSM
	v_vecTangentLightPos = matTBN * (v_vecFragPos - u_vecLightDirection);
#else // SUBTYPE_DIR || SUBTYPE_CSM
	v_vecTangentLightPos = matTBN * u_vecLightPosition;
#endif // SUBTYPE_DIR || SUBTYPE_CSM
	v_vecTangentViewPos = matTBN * u_vecViewPos;
	v_vecTangentFragPos = matTBN * v_vecFragPos;
	
#if !SUBTYPE_POINT
#if SUBTYPE_CSM
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
#else // SUBTYPE_CSM
	vec4 vecFragPosLightSpace = u_matShadowMatrix[0] * vec4(v_vecFragPos, 1.0f);
	v_flShadowMapFactor = vecFragPosLightSpace.w;
	v_vecShadowMapCoords = vecFragPosLightSpace.xy;
#if SUBTYPE_DIR
	v_flShadowMapDepth = vecFragPosLightSpace.z;
#endif // SUBTYPE_DIR
#endif // SUBTYPE_CSM
#endif // !SUBTYPE_POINT
}
