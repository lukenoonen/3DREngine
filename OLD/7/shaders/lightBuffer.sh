layout (std140) uniform lightBuffer
{
	vec3 u_vecLightAmbient;
	vec3 u_vecLightDiffuse;
	vec3 u_vecLightSpecular;
	bool u_bLightCastShadows;
};