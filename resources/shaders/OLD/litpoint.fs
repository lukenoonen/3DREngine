#version 330 core

#subshader QUALITY

#subshader SHADOW

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

#include "lightBuffer.sh"
#include "lightPositionBuffer.sh"
#include "lightMaxDistance.sh"
#include "lightPointBuffer.sh"
#if SHADOW_TRUE
#include "shadowBlurBuffer.sh"
#include "shadowFadeBuffer.sh"
#include "vogelSample.sh"
#include "bias.sh"
#endif // SHADOW_TRUE

uniform sampler2D u_sDiffuse;
#if !QUALITY_LOW
uniform bool u_bUseSpecular;
uniform sampler2D u_sSpecular;
#endif // !QUALITY_LOW
uniform bool u_bUseNormal;
uniform sampler2D u_sNormal;
uniform float u_flShininess;
#if SHADOW_TRUE
uniform samplerCubeShadow u_sShadowMap;
#endif // SHADOW_TRUE

out vec4 v_vecFragColor;

#if SHADOW_TRUE
vec2 GetSampleCoordinate(vec3 vecFragToLight) // OPTIMIZE / CLEAN THIS UP!!
{
	vec2 vecCoord;
	float flSlice;

	vec3 vecAbsFragToLight = abs(vecFragToLight);
	float flMaxCoord = max(vecAbsFragToLight.x, max(vecAbsFragToLight.y, vecAbsFragToLight.z));
	if (flMaxCoord == vecAbsFragToLight.y)
	{
		if (vecFragToLight.y > 0.0f)
		{
			flSlice = 0.0f;
		}
		else
		{
			vecFragToLight.z = -vecFragToLight.z;
			flSlice = 2.0f;
		}
		vecCoord = vecFragToLight.xz / vecFragToLight.y;
	}
	else if (flMaxCoord == vecAbsFragToLight.x)
	{
		vecFragToLight.y = -vecFragToLight.y;
		if (vecFragToLight.x > 0.0f)
		{
			flSlice = 1.0f;
		}
		else
		{
			vecFragToLight.z = -vecFragToLight.z;
			flSlice = 3.0f;
		}
		vecCoord = vecFragToLight.yz / vecFragToLight.x;
	}
	else
	{
		vecFragToLight.x = -vecFragToLight.x;
		if (vecFragToLight.z > 0.0f)
		{
			flSlice = 4.0f;
		}
		else
		{
			vecFragToLight.y = -vecFragToLight.y;
			flSlice = 5.0f;
		}
		vecCoord = vecFragToLight.xy / vecFragToLight.z;
	}

	vecCoord = vecCoord * 0.5f + 0.5f;
	vecCoord = vecCoord * (1.0f - 2.0f * u_flShadowBlurScale) + u_flShadowBlurScale;
	vecCoord.x = vecCoord.x / 6.0f + flSlice / 6.0f;
	
	return vecCoord;
}

vec2 ToDirection(vec2 vecPolar)
{
	float flSinTheta = sin(vecPolar.y);
	return vec3(cos(vecPolar.x) * flSinTheta, sin(vecPolar.x) * flSinTheta, cos(vecPolar.y));
}

void Test()
{
	float flShadow = 1.0f;
#if SHADOW_TRUE
	if (v_flClipSpacePosZ < u_flShadowFadeFar)
	{
#if QUALITY_HIGH || QUALITY_ULTRA
		float flPhi = InterleavedGradientNoise(v_vecFragPos) * 6.28318530718f;
#else // QUALITY_HIGH || QUALITY_ULTRA
		float flPhi = 0.0f;
#endif // QUALITY_HIGH || QUALITY_ULTRA
		float flTheta = acos(vecNormalizedDelta.z);
		vec2 vecPolar = vec2(acos(vecDirection.x / sin(flTheta)), flTheta);
		flShadow = 0.0f;
		for (int i = 0; i < SHADOW_SAMPLES; i++)
			flShadow += texture(u_sShadowMap, vec4(ToDirection(vecPolar + VogelDiskSample(i, SHADOW_SAMPLES, u_flShadowBlurScale, flPhi)), flAdjustedShadowMapDepth));
		
		flShadow *= SHADOW_SAMPLES_INV_F;
		
		if (v_flClipSpacePosZ > u_flShadowFadeNear)
			flShadow = mix(flShadow, 1.0f, (v_flClipSpacePosZ - u_flShadowFadeNear) / (u_flShadowFadeFar - u_flShadowFadeNear));
	}
#endif // SHADOW_TRUE
}

#endif // SHADOW_TRUE

void main()
{
	vec3 vecDiffuseSample = texture(u_sDiffuse, v_vecTexCoords).xyz;
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
	
	vec3 vecNormal = u_bUseNormal ? normalize((texture(u_sNormal, v_vecTexCoords).rgb * 2.0f - 1.0f) * vec3(1.0f, -1.0f, 1.0f)) : vec3(0.0f, 0.0f, 1.0f);
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
		vec2 vecCoord = GetSampleCoordinate(-vecDelta);
		float flAdjustedShadowMapDepth = flDistance / u_flLightMaxDistance - bias(flNormalDirectionDot);
		flShadow = 0.0f;
		for (int i = 0; i < SHADOW_SAMPLES; i++)
			flShadow += texture(u_sShadowMap, vec3(vecCoord + VogelDiskSample(i, SHADOW_SAMPLES, u_flShadowBlurScale, flPhi) * vec2(0.16666666666f, 1.0f), flAdjustedShadowMapDepth));
		
		flShadow *= SHADOW_SAMPLES_INV_F;
		
		if (v_flClipSpacePosZ > u_flShadowFadeNear)
			flShadow = mix(flShadow, 1.0f, (v_flClipSpacePosZ - u_flShadowFadeNear) / (u_flShadowFadeFar - u_flShadowFadeNear));
	}
#endif // SHADOW_TRUE

#if QUALITY_LOW
    v_vecFragColor = vec4(vecAmbientLight + flShadow * (vecDiffuseLight), 1.0f);
#else // QUALITY_LOW
    v_vecFragColor = vec4(vecAmbientLight + flShadow * (vecDiffuseLight + vecSpecularLight), 1.0f);
#endif // QUALITY_LOW
}