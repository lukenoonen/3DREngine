#version 330 core

#subshader QUALITY
#subshader SHADOW
#subshader REFLECTION

#if QUALITY_LOW
#define SHADOW_SAMPLES 4
#define SHADOW_SAMPLES_INV_F 0.25f
#elif QUALITY_MEDIUM
#define SHADOW_SAMPLES 4
#define SHADOW_SAMPLES_INV_F 0.25f
#elif QUALITY_HIGH
#define SHADOW_SAMPLES 8
#define SHADOW_SAMPLES_INV_F 0.125f
#elif QUALITY_ULTRA
#define SHADOW_SAMPLES 16
#define SHADOW_SAMPLES_INV_F 0.0625f;
#endif // QUALITY_LOW

in vec2 v_vecTexCoords;
in float v_flClipSpacePosZ;
in vec3 v_vecFragPos;
in vec3 v_vecNormal;
in vec3 v_vecTangentLightPos;
in vec3 v_vecTangentViewPos;
in vec3 v_vecTangentFragPos;
#if REFLECTION_TRUE
in float v_flReflectionMapFactor;
in vec2 v_vecReflectionMapCoords;
#endif // REFLECTION_TRUE

#include "lightBuffer"
#include "lightPositionBuffer"
#include "lightMaxDistance"
#include "lightPointBuffer"
#if SHADOW_TRUE
#include "shadowBlurBuffer"
#include "shadowFadeBuffer"
#include "vogelSample"
#include "bias"
#endif // SHADOW_TRUE

uniform sampler2D u_sDiffuse;
#if !QUALITY_LOW
uniform bool u_bUseSpecular;
uniform sampler2D u_sSpecular;
uniform float u_flShininess;
uniform bool u_bUseNormal;
uniform sampler2D u_sNormal;
#endif // !QUALITY_LOW
uniform bool u_bUseCamera;
uniform sampler2D u_sCamera;
uniform sampler2D u_sCameraTexture;
#if SHADOW_TRUE
uniform samplerCubeShadow u_sShadow;
#endif // SHADOW_TRUE

out vec4 v_vecFragColor;

#if SHADOW_TRUE
vec3 ToDirection(vec2 vecPolar)
{
	float flSinPhi = sin(vecPolar.y);
	return vec3(cos(vecPolar.x) * flSinPhi, sin(vecPolar.x) * flSinPhi, cos(vecPolar.y));
}
#endif // SHADOW_TRUE

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
	vec3 vecDelta = u_vecLightPosition - v_vecFragPos;
	float flDistance = length(vecDelta);
	vec3 vecNormalizedDelta = vecDelta / flDistance;
	
	if (flDistance > u_flLightMaxDistance)
	{
		v_vecFragColor = vec4(0.0f);
		return;
	}

	float flAttenuation = 1.0f / ((u_flLightConstant + u_flLightLinear * flDistance + u_flLightQuadratic * flDistance * flDistance));
	vec3 vecAmbientLight = u_vecLightAmbient * vecDiffuseSample * flAttenuation;
	float flNormalDirectionDot = dot(vecNormalizedDelta, v_vecNormal);
	
	if (flNormalDirectionDot < 0.000001f)
	{
		v_vecFragColor = vec4(vecAmbientLight, 1.0f);
		return;
	}
	
#if !QUALITY_LOW
	vec3 vecNormal = u_bUseNormal ? normalize((texture(u_sNormal, v_vecTexCoords).rgb * 2.0f - 1.0f) * vec3(1.0f, -1.0f, 1.0f)) : vec3(0.0f, 0.0f, 1.0f);
#else // !QUALITY_LOW
	vec3 vecNormal = vec3(0.0f, 0.0f, 1.0f);
#endif // !QUALITY_LOW
	vec3 vecLightDirection = normalize(v_vecTangentLightPos - v_vecTangentFragPos);
	
	vec3 vecDiffuseLight = u_vecLightDiffuse * max(dot(vecNormal, vecLightDirection), 0.0f) * vecDiffuseSample * flAttenuation;
#if !QUALITY_LOW
	vec3 vecSpecularLight = u_bUseSpecular ? u_vecLightSpecular * pow(max(dot(vecNormal, (vecLightDirection + normalize(v_vecTangentViewPos - v_vecTangentFragPos)) * 0.5f), 0.0f), u_flShininess) * texture(u_sSpecular, v_vecTexCoords).xyz * flAttenuation : vec3(0.0f);
#endif // !QUALITY_LOW

	float flShadow = 1.0f;
#if SHADOW_TRUE
	if (v_flClipSpacePosZ < u_flShadowFadeFar)
	{
#if QUALITY_HIGH || QUALITY_ULTRA
		float flPhi = InterleavedGradientNoise(v_vecFragPos) * 6.28318530718f;
#else // QUALITY_HIGH || QUALITY_ULTRA
		float flPhi = 0.0f;
#endif // QUALITY_HIGH || QUALITY_ULTRA
		vec2 vecPolar = vec2(atan(vecNormalizedDelta.y, vecNormalizedDelta.x), acos(vecNormalizedDelta.z));
		float flAdjustedShadowMapDepth = flDistance / u_flLightMaxDistance - bias(flNormalDirectionDot);
		flShadow = 0.0f;
		for (int i = 0; i < SHADOW_SAMPLES; i++)
			flShadow += texture(u_sShadow, vec4(ToDirection(vecPolar + VogelDiskSample(i, SHADOW_SAMPLES, u_flShadowBlurScale, flPhi)).xzy, flAdjustedShadowMapDepth));
		
		flShadow *= SHADOW_SAMPLES_INV_F;
		
		if (v_flClipSpacePosZ > u_flShadowFadeNear)
			flShadow = mix(flShadow, 1.0f, (v_flClipSpacePosZ - u_flShadowFadeNear) / (u_flShadowFadeFar - u_flShadowFadeNear));
	}
#endif // SHADOW_TRUE

#if !QUALITY_LOW
    v_vecFragColor = vec4(vecAmbientLight + flShadow * (vecDiffuseLight + vecSpecularLight), 1.0f);
#else // !QUALITY_LOW
    v_vecFragColor = vec4(vecAmbientLight + flShadow * vecDiffuseLight, 1.0f);
#endif // !QUALITY_LOW
}