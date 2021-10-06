#version 330 core

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

uniform vec2 u_vecTextureScale;

out vec2 v_vecTexCoords;

void main()
{
	v_vecTexCoords = a_vecTexCoords * u_vecTextureScale;
	
	mat4 matBoneTransform = (a_vecWeights.x != 0.0) ? (u_matBones[a_vecBoneIDs.w] * a_vecWeights.w + u_matBones[a_vecBoneIDs.x] * a_vecWeights.x + u_matBones[a_vecBoneIDs.y] * a_vecWeights.y + u_matBones[a_vecBoneIDs.z] * a_vecWeights.z) : mat4(1.0);
	gl_Position = u_matProjectionView * u_matModel * matBoneTransform * vec4(a_vecPos, 1.0);
}