#version 330 core

#subshader ANIMATE

layout (location = 0) in vec3 a_vecPos;
layout (location = 5) in ivec4 a_vecBoneIDs;
layout (location = 6) in vec4 a_vecWeights;

#include "modelBuffer.sh"
#include "bonesBuffer.sh"
#include "shadowBuffer.sh"

void main()
{
#if ANIMATE_TRUE
	gl_Position = u_matShadowMatrix[0] * u_matModel * (u_matBones[a_vecBoneIDs.w] * a_vecWeights.w + u_matBones[a_vecBoneIDs.x] * a_vecWeights.x + u_matBones[a_vecBoneIDs.y] * a_vecWeights.y + u_matBones[a_vecBoneIDs.z] * a_vecWeights.z) * vec4(a_vecPos, 1.0);
#else // ANIMATE_TRUE
	gl_Position = u_matShadowMatrix[0] * u_matModel * vec4(a_vecPos, 1.0);
#endif // ANIMATE_TRUE
}