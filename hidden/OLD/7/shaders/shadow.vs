#version 330 core

#subtype SUBTYPE_DIR
#subtype SUBTYPE_POINT
#subtype SUBTYPE_SPOT
#subtype SUBTYPE_CSM

layout (location = 0) in vec3 a_vecPos;
layout (location = 5) in ivec4 a_vecBoneIDs;
layout (location = 6) in vec4 a_vecWeights;

#include "modelBuffer.sh"
#include "bonesBuffer.sh"
#include "shadowBuffer.sh"

#if SUBTYPE_POINT || SUBTYPE_SPOT
out vec3 v_vecFragPos;
#endif // SUBTYPE_POINT || SUBTYPE_SPOT

void main()
{
	mat4 matBoneTransform = (a_vecWeights.x != 0.0) ? (u_matBones[a_vecBoneIDs.w] * a_vecWeights.w + u_matBones[a_vecBoneIDs.x] * a_vecWeights.x + u_matBones[a_vecBoneIDs.y] * a_vecWeights.y + u_matBones[a_vecBoneIDs.z] * a_vecWeights.z) : mat4(1.0);
	
	gl_Position = u_matShadowMatrix[0] * u_matModel * matBoneTransform * vec4(a_vecPos, 1.0);
#if SUBTYPE_POINT || SUBTYPE_SPOT
	v_vecFragPos = (u_matModel * matBoneTransform * vec4(a_vecPos, 1.0)).xyz;
#endif // SUBTYPE_POINT || SUBTYPE_SPOT
}