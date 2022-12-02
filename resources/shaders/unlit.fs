#version 330 core

#subshader REFLECTION

in vec2 v_vecTexCoords;
#if REFLECTION_TRUE
in float v_flReflectionMapFactor;
in vec2 v_vecReflectionMapCoords;
#endif // REFLECTION_TRUE

uniform sampler2D u_sDiffuse;
uniform bool u_bUseCamera;
uniform sampler2D u_sCamera;
uniform sampler2D u_sCameraTexture;

out vec4 v_vecFragColor;

void main()
{
	vec3 vecDiffuseSample;
	if (u_bUseCamera)
	{
		vec3 vecCameraSample = texture(u_sCamera, v_vecTexCoords).xyz;
#if REFLECTION_TRUE
		vecDiffuseSample = texture(u_sDiffuse, v_vecTexCoords).xyz * (1.0f - vecCameraSample.r) + texture(u_sCameraTexture, (v_vecReflectionMapCoords / v_flReflectionMapFactor) * 0.5f + 0.5f).xyz * vecCameraSample.r;
#else // REFLECTION_TRUE
		vecDiffuseSample = texture(u_sDiffuse, v_vecTexCoords).xyz * (1.0f - vecCameraSample.r) + texture(u_sCameraTexture, v_vecTexCoords).xyz * vecCameraSample.r;
#endif // REFLECTION_TRUE
	}
	else
	{
		vecDiffuseSample = texture(u_sDiffuse, v_vecTexCoords).xyz;
	}
    v_vecFragColor = vec4(vecDiffuseSample, 1.0);
}