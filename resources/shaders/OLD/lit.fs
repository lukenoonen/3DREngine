#version 330 core

#subtype SUBTYPE_DIR
#subtype SUBTYPE_POINT
#subtype SUBTYPE_SPOT
#subtype SUBTYPE_CSM

#quality QUALITY_LOW
#quality QUALITY_MEDIUM
#quality QUALITY_HIGH
#quality QUALITY_ULTRA

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
#if !SUBTYPE_POINT
#if SUBTYPE_CSM
in float v_flShadowMapFactor[4];
in vec2 v_vecShadowMapCoords[4];
in float v_flShadowMapDepth[4];
#else // SUBTYPE_CSM
in float v_flShadowMapFactor;
in vec2 v_vecShadowMapCoords;
#if SUBTYPE_DIR
in float v_flShadowMapDepth;
#endif // SUBTYPE_DIR
#endif // SUBTYPE_CSM
#endif // !SUBTYPE_POINT

#include "lightBuffer.sh"
#include "lightPositionBuffer.sh"
#if !SUBTYPE_POINT
#include "lightDirectionBuffer.sh"
#endif // !SUBTYPE_POINT
#if SUBTYPE_POINT || SUBTYPE_SPOT
#include "lightMaxDistance.sh"
#include "lightPointBuffer.sh"
#if SUBTYPE_SPOT
#include "lightSpotBuffer.sh"
#endif // SUBTYPE_SPOT
#endif // SUBTYPE_POINT || SUBTYPE_SPOT
#if SUBTYPE_CSM
#include "shadowCascadeFadeBuffer.sh"
#endif // SUBTYPE_CSM
#include "shadowBlurBuffer.sh"
#include "shadowFadeBuffer.sh"

uniform sampler2D u_sDiffuse;
uniform bool u_bUseSpecular;
uniform sampler2D u_sSpecular;
uniform bool u_bUseNormal;
uniform sampler2D u_sNormal;
uniform float u_flShininess;
#if SUBTYPE_CSM
uniform sampler2DShadow u_sShadowMap[4];
#else
uniform sampler2DShadow u_sShadowMap;
#endif // SUBTYPE_POINT

out vec4 v_vecFragColor;

vec2 VogelDiskSample(int iSampleIndex, int iSamplesCount, int iShadowMapIndex)
{
	float flRadius = sqrt((float(iSampleIndex) + 0.5f) / (float(iSamplesCount) + 0.5f)) * u_vecShadowBlurScale[iShadowMapIndex];
	float flTheta = iSampleIndex * 2.39996322972865332f;

#if SUBTYPE_POINT
	return vec2(flRadius * sin(flTheta) / 6.0f, flRadius * cos(flTheta));
#else // SUBTYPE_POINT
	return vec2(flRadius * sin(flTheta), flRadius * cos(flTheta));
#endif // SUBTYPE_POINT
}

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
	vecCoord = vecCoord * (1.0f - 2.0f * u_vecShadowBlurScale[0]) + u_vecShadowBlurScale[0];
	vecCoord.x = vecCoord.x / 6.0f + flSlice / 6.0f;
	
	return vecCoord;
}

void main()
{
	vec3 vecDiffuseSample = texture(u_sDiffuse, v_vecTexCoords).xyz;
	
#if SUBTYPE_POINT || SUBTYPE_SPOT
	vec3 vecDelta = u_vecLightPosition - v_vecFragPos;
	float flDistance = length(vecDelta);
	vec3 vecNormalizedDelta = vecDelta / flDistance;
	if (flDistance > u_flLightMaxDistance)
	{
		v_vecFragColor = vec4(0.0f);
		return;
	}

	float flAttenuation = 1.0f / ((u_flLightConstant + u_flLightLinear * flDistance + u_flLightQuadratic * flDistance * flDistance));
#if SUBTYPE_SPOT
	flAttenuation *= clamp((dot(vecNormalizedDelta, u_vecLightDirection) + u_flLightOuterCutOff) / (u_flLightOuterCutOff - u_flLightCutOff), 0.0f, 1.0f);
	if (flAttenuation == 0.0f)
	{
		v_vecFragColor = vec4(0.0f);
		return;
	}
		
#endif // SUBTYPE_SPOT
	vec3 vecAmbientLight = u_vecLightAmbient * vecDiffuseSample * flAttenuation;
	
	float flNormalDirectionDot = dot(vecNormalizedDelta, v_vecNormal);
#else // SUBTYPE_POINT || SUBTYPE_SPOT
	vec3 vecAmbientLight = u_vecLightAmbient * vecDiffuseSample;
	
	float flNormalDirectionDot = dot(-u_vecLightDirection, v_vecNormal);
#endif // SUBTYPE_POINT || SUBTYPE_SPOT
	if (flNormalDirectionDot < 0.000001f)
	{
		v_vecFragColor = vec4(vecAmbientLight, 1.0f);
		return;
	}
	
	vec3 vecNormal = u_bUseNormal ? normalize((texture(u_sNormal, v_vecTexCoords).rgb * 2.0f - 1.0f) * vec3(1.0f, -1.0f, 1.0f)) : vec3(0.0f, 0.0f, 1.0f);
	vec3 vecLightDirection = normalize(v_vecTangentLightPos - v_vecTangentFragPos);
#if !QUALITY_LOW
	vec3 vecViewDirection = normalize(v_vecTangentViewPos - v_vecTangentFragPos);
#endif // !QUALITY_LOW
	
#if SUBTYPE_POINT || SUBTYPE_SPOT
	vec3 vecDiffuseLight = u_vecLightDiffuse * max(dot(vecNormal, vecLightDirection), 0.0f) * vecDiffuseSample * flAttenuation;
#if !QUALITY_LOW
	vec3 vecSpecularLight = u_bUseSpecular ? u_vecLightSpecular * pow(max(dot(vecNormal, (vecLightDirection + vecViewDirection) * 0.5f), 0.0f), u_flShininess) * texture(u_sSpecular, v_vecTexCoords).xyz * flAttenuation : vec3(0.0f);
#endif // !QUALITY_LOW
#else // SUBTYPE_POINT || SUBTYPE_SPOT
	vec3 vecDiffuseLight = u_vecLightDiffuse * max(dot(vecNormal, vecLightDirection), 0.0f) * vecDiffuseSample;
#if !QUALITY_LOW
	vec3 vecSpecularLight = u_bUseSpecular ? u_vecLightSpecular * pow(max(dot(vecNormal, (vecLightDirection + vecViewDirection) * 0.5f), 0.0f), u_flShininess) * texture(u_sSpecular, v_vecTexCoords).xyz : vec3(0.0f);
#endif // !QUALITY_LOW
#endif // SUBTYPE_POINT || SUBTYPE_SPOT

	float flShadow = 1.0f;
	if (u_bLightCastShadows && v_flClipSpacePosZ < u_flShadowFadeFar)
	{
#if SUBTYPE_DIR
		float flAdjustedShadowMapDepth = (v_flShadowMapDepth / v_flShadowMapFactor) * 0.5f + 0.5f;
		if (flAdjustedShadowMapDepth <= 1.0f)
		{
			vec2 vecAdjustedShadowMapCoords = (v_vecShadowMapCoords / v_flShadowMapFactor) * 0.5f + 0.5f;
			flAdjustedShadowMapDepth -= max(0.004f * (1.0f - flNormalDirectionDot), 0.001f);
			flShadow = 0.0f;
			for (int i = 0; i < SHADOW_SAMPLES; i++)
				flShadow += texture(u_sShadowMap, vec3(vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, 0), flAdjustedShadowMapDepth));
		
			flShadow *= SHADOW_SAMPLES_INV_F;
		}
#elif SUBTYPE_POINT
		vec2 vecCoord = GetSampleCoordinate(-vecDelta);
		float flAdjustedShadowMapDepth = flDistance / u_flLightMaxDistance - max(0.004f * (1.0f - flNormalDirectionDot * flNormalDirectionDot), 0.001f);
		flShadow = 0.0f;
		for (int i = 0; i < SHADOW_SAMPLES; i++)
			flShadow += texture(u_sShadowMap, vec3(vecCoord + VogelDiskSample(i, SHADOW_SAMPLES, 0), flAdjustedShadowMapDepth));
		
		flShadow *= SHADOW_SAMPLES_INV_F;
#elif SUBTYPE_SPOT
		vec2 vecAdjustedShadowMapCoords = (v_vecShadowMapCoords / v_flShadowMapFactor) * 0.5f + 0.5f;
		float flAdjustedShadowMapDepth = flDistance / u_flLightMaxDistance - max(0.004f * (1.0f - flNormalDirectionDot * flNormalDirectionDot), 0.001f);
		flShadow = 0.0f;
		for (int i = 0; i < SHADOW_SAMPLES; i++)
			flShadow += texture(u_sShadowMap, vec3(vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, 0), flAdjustedShadowMapDepth));
		
		flShadow *= SHADOW_SAMPLES_INV_F;
#elif SUBTYPE_CSM
		float flBias = max(0.004f * (1.0f - flNormalDirectionDot), 0.001f);
		if (v_flClipSpacePosZ <= u_vecShadowCascadeFadeFar[0])
		{
			float flAdjustedShadowMapDepth = (v_flShadowMapDepth[0] / v_flShadowMapFactor[0]) * 0.5f + 0.5f - flBias;
			vec2 vecAdjustedShadowMapCoords = (v_vecShadowMapCoords[0] / v_flShadowMapFactor[0]) * 0.5f + 0.5f;
			
			flShadow = 0.0f;
			for (int i = 0; i < SHADOW_SAMPLES; i++)
				flShadow += texture(u_sShadowMap[0], vec3(vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, 0), flAdjustedShadowMapDepth));
			
			flShadow *= SHADOW_SAMPLES_INV_F;
			
			if (u_vecShadowCascadeFadeNear[0] < u_vecShadowCascadeFadeFar[0] && v_flClipSpacePosZ > u_vecShadowCascadeFadeNear[0])
			{
				float flAdjustedShadowMapDepthFar = (v_flShadowMapDepth[1] / v_flShadowMapFactor[1]) * 0.5f + 0.5f - flBias;
				vec2 vecAdjustedShadowMapCoordsFar = (v_vecShadowMapCoords[1] / v_flShadowMapFactor[1]) * 0.5f + 0.5f;
				
				float flShadowFar = 0.0f;
				for (int i = 0; i < SHADOW_SAMPLES; i++)
					flShadowFar += texture(u_sShadowMap[1], vec3(vecAdjustedShadowMapCoordsFar + VogelDiskSample(i, SHADOW_SAMPLES, 1), flAdjustedShadowMapDepthFar));
				
				flShadowFar *= SHADOW_SAMPLES_INV_F;
				
				flShadow = mix(flShadow, flShadowFar, (v_flClipSpacePosZ - u_vecShadowCascadeFadeNear[0]) / (u_vecShadowCascadeFadeFar[0] - u_vecShadowCascadeFadeNear[0]));
			}
		}
		else if (v_flClipSpacePosZ <= u_vecShadowCascadeFadeFar[1])
		{
			float flAdjustedShadowMapDepth = (v_flShadowMapDepth[1] / v_flShadowMapFactor[1]) * 0.5f + 0.5f - flBias;
			vec2 vecAdjustedShadowMapCoords = (v_vecShadowMapCoords[1] / v_flShadowMapFactor[1]) * 0.5f + 0.5f;
			
			flShadow = 0.0f;
			for (int i = 0; i < SHADOW_SAMPLES; i++)
				flShadow += texture(u_sShadowMap[1], vec3(vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, 1), flAdjustedShadowMapDepth));
			
			flShadow *= SHADOW_SAMPLES_INV_F;
			
			if (u_vecShadowCascadeFadeNear[1] < u_vecShadowCascadeFadeFar[1] && v_flClipSpacePosZ > u_vecShadowCascadeFadeNear[1])
			{
				float flAdjustedShadowMapDepthFar = (v_flShadowMapDepth[2] / v_flShadowMapFactor[2]) * 0.5f + 0.5f - flBias;
				vec2 vecAdjustedShadowMapCoordsFar = (v_vecShadowMapCoords[2] / v_flShadowMapFactor[2]) * 0.5f + 0.5f;
				
				float flShadowFar = 0.0f;
				for (int i = 0; i < SHADOW_SAMPLES; i++)
					flShadowFar += texture(u_sShadowMap[2], vec3(vecAdjustedShadowMapCoordsFar + VogelDiskSample(i, SHADOW_SAMPLES, 2), flAdjustedShadowMapDepthFar));
				
				flShadowFar *= SHADOW_SAMPLES_INV_F;
				
				flShadow = mix(flShadow, flShadowFar, (v_flClipSpacePosZ - u_vecShadowCascadeFadeNear[1]) / (u_vecShadowCascadeFadeFar[1] - u_vecShadowCascadeFadeNear[1]));
			}
		}
		else if (v_flClipSpacePosZ <= u_vecShadowCascadeFadeFar[2])
		{
			float flAdjustedShadowMapDepth = (v_flShadowMapDepth[2] / v_flShadowMapFactor[2]) * 0.5f + 0.5f - flBias;
			vec2 vecAdjustedShadowMapCoords = (v_vecShadowMapCoords[2] / v_flShadowMapFactor[2]) * 0.5f + 0.5f;
			
			flShadow = 0.0f;
			for (int i = 0; i < SHADOW_SAMPLES; i++)
				flShadow += texture(u_sShadowMap[2], vec3(vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, 2), flAdjustedShadowMapDepth));
			
			flShadow *= SHADOW_SAMPLES_INV_F;
			
			if (u_vecShadowCascadeFadeNear[2] < u_vecShadowCascadeFadeFar[2] && v_flClipSpacePosZ > u_vecShadowCascadeFadeNear[2])
			{
				float flAdjustedShadowMapDepthFar = (v_flShadowMapDepth[3] / v_flShadowMapFactor[3]) * 0.5f + 0.5f - flBias;
				vec2 vecAdjustedShadowMapCoordsFar = (v_vecShadowMapCoords[3] / v_flShadowMapFactor[3]) * 0.5f + 0.5f;
				
				float flShadowFar = 0.0f;
				for (int i = 0; i < SHADOW_SAMPLES; i++)
					flShadowFar += texture(u_sShadowMap[3], vec3(vecAdjustedShadowMapCoordsFar + VogelDiskSample(i, SHADOW_SAMPLES, 3), flAdjustedShadowMapDepthFar));
				
				flShadowFar *= SHADOW_SAMPLES_INV_F;
				
				flShadow = mix(flShadow, flShadowFar, (v_flClipSpacePosZ - u_vecShadowCascadeFadeNear[2]) / (u_vecShadowCascadeFadeFar[2] - u_vecShadowCascadeFadeNear[2]));
			}
		}
		else if (v_flClipSpacePosZ <= u_vecShadowCascadeFadeFar[3])
		{
			float flAdjustedShadowMapDepth = (v_flShadowMapDepth[3] / v_flShadowMapFactor[3]) * 0.5f + 0.5f - flBias;
			vec2 vecAdjustedShadowMapCoords = (v_vecShadowMapCoords[3] / v_flShadowMapFactor[3]) * 0.5f + 0.5f;
			
			flShadow = 0.0f;
			for (int i = 0; i < SHADOW_SAMPLES; i++)
				flShadow += texture(u_sShadowMap[3], vec3(vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, 3), flAdjustedShadowMapDepth));
			
			flShadow *= SHADOW_SAMPLES_INV_F;
		}
		
#endif // SUBTYPE_DIR
		if (v_flClipSpacePosZ > u_flShadowFadeNear)
			flShadow = mix(flShadow, 1.0f, (v_flClipSpacePosZ - u_flShadowFadeNear) / (u_flShadowFadeFar - u_flShadowFadeNear));
	}
	
#if QUALITY_LOW
    v_vecFragColor = vec4(vecAmbientLight + flShadow * (vecDiffuseLight), 1.0f);
#else // QUALITY_LOW
    v_vecFragColor = vec4(vecAmbientLight + flShadow * (vecDiffuseLight + vecSpecularLight), 1.0f);
#endif // QUALITY_LOW
}