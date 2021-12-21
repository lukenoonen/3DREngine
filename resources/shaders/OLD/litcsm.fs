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
#if SHADOW_TRUE
in float v_flShadowMapFactor[4];
in vec2 v_vecShadowMapCoords[4];
in float v_flShadowMapDepth[4];
#endif // SHADOW_TRUE

#include "lightBuffer.sh"
#include "lightPositionBuffer.sh"
#include "lightDirectionBuffer.sh"
#if SHADOW_TRUE
#include "shadowCascadeFadeBuffer.sh"
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
uniform sampler2DShadow u_sShadowMap;
#endif // SHADOW_TRUE

out vec4 v_vecFragColor;

void main()
{
	vec3 vecDiffuseSample = texture(u_sDiffuse, v_vecTexCoords).xyz;
	vec3 vecAmbientLight = u_vecLightAmbient * vecDiffuseSample;
	float flNormalDirectionDot = dot(-u_vecLightDirection, v_vecNormal);
	
	if (flNormalDirectionDot < 0.000001f)
	{
		v_vecFragColor = vec4(vecAmbientLight, 1.0f);
		return;
	}
	
	vec3 vecNormal = u_bUseNormal ? normalize((texture(u_sNormal, v_vecTexCoords).rgb * 2.0f - 1.0f) * vec3(1.0f, -1.0f, 1.0f)) : vec3(0.0f, 0.0f, 1.0f);
	vec3 vecLightDirection = normalize(v_vecTangentLightPos - v_vecTangentFragPos);

	vec3 vecDiffuseLight = u_vecLightDiffuse * max(dot(vecNormal, vecLightDirection), 0.0f) * vecDiffuseSample;
#if !QUALITY_LOW
	vec3 vecSpecularLight = u_bUseSpecular ? u_vecLightSpecular * pow(max(dot(vecNormal, (vecLightDirection + normalize(v_vecTangentViewPos - v_vecTangentFragPos)) * 0.5f), 0.0f), u_flShininess) * texture(u_sSpecular, v_vecTexCoords).xyz : vec3(0.0f);
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
		float flBias = bias(flNormalDirectionDot);
		if (v_flClipSpacePosZ <= u_vecShadowCascadeFadeFar[0])
		{
			float flAdjustedShadowMapDepth = (v_flShadowMapDepth[0] / v_flShadowMapFactor[0]) * 0.5f + 0.5f - flBias;
			vec2 vecAdjustedShadowMapCoords = (v_vecShadowMapCoords[0] / v_flShadowMapFactor[0]) * 0.5f + 0.5f;
			
			flShadow = 0.0f;
			for (int i = 0; i < SHADOW_SAMPLES; i++)
				flShadow += texture(u_sShadowMap, vec3((vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, u_flShadowBlurScale, flPhi)) * vec2(0.25f, 1.0f), flAdjustedShadowMapDepth));
			
			flShadow *= SHADOW_SAMPLES_INV_F;
			
			if (u_vecShadowCascadeFadeNear[0] < u_vecShadowCascadeFadeFar[0] && v_flClipSpacePosZ > u_vecShadowCascadeFadeNear[0])
			{
				float flAdjustedShadowMapDepthFar = (v_flShadowMapDepth[1] / v_flShadowMapFactor[1]) * 0.5f + 0.5f - flBias;
				vec2 vecAdjustedShadowMapCoordsFar = (v_vecShadowMapCoords[1] / v_flShadowMapFactor[1]) * 0.5f + 0.5f;
				
				float flShadowFar = 0.0f;
				for (int i = 0; i < SHADOW_SAMPLES; i++)
					flShadowFar += texture(u_sShadowMap, vec3((vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, u_flShadowBlurScale, flPhi)) * vec2(0.25f, 1.0f) + vec2(0.25f, 0.0f), flAdjustedShadowMapDepthFar));
				
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
				flShadow += texture(u_sShadowMap, vec3((vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, u_flShadowBlurScale, flPhi)) * vec2(0.25f, 1.0f) + vec2(0.25f, 0.0f), flAdjustedShadowMapDepth));
			
			flShadow *= SHADOW_SAMPLES_INV_F;
			
			if (u_vecShadowCascadeFadeNear[1] < u_vecShadowCascadeFadeFar[1] && v_flClipSpacePosZ > u_vecShadowCascadeFadeNear[1])
			{
				float flAdjustedShadowMapDepthFar = (v_flShadowMapDepth[2] / v_flShadowMapFactor[2]) * 0.5f + 0.5f - flBias;
				vec2 vecAdjustedShadowMapCoordsFar = (v_vecShadowMapCoords[2] / v_flShadowMapFactor[2]) * 0.5f + 0.5f;
				
				float flShadowFar = 0.0f;
				for (int i = 0; i < SHADOW_SAMPLES; i++)
					flShadowFar += texture(u_sShadowMap, vec3((vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, u_flShadowBlurScale, flPhi)) * vec2(0.25f, 1.0f) + vec2(0.5f, 0.0f), flAdjustedShadowMapDepthFar));
				
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
				flShadow += texture(u_sShadowMap[2], vec3((vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, u_flShadowBlurScale, flPhi)) * vec2(0.25f, 1.0f) + vec2(0.5f, 0.0f), flAdjustedShadowMapDepth));
			
			flShadow *= SHADOW_SAMPLES_INV_F;
			
			if (u_vecShadowCascadeFadeNear[2] < u_vecShadowCascadeFadeFar[2] && v_flClipSpacePosZ > u_vecShadowCascadeFadeNear[2])
			{
				float flAdjustedShadowMapDepthFar = (v_flShadowMapDepth[3] / v_flShadowMapFactor[3]) * 0.5f + 0.5f - flBias;
				vec2 vecAdjustedShadowMapCoordsFar = (v_vecShadowMapCoords[3] / v_flShadowMapFactor[3]) * 0.5f + 0.5f;
				
				float flShadowFar = 0.0f;
				for (int i = 0; i < SHADOW_SAMPLES; i++)
					flShadowFar += texture(u_sShadowMap, vec3((vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, u_flShadowBlurScale, flPhi)) * vec2(0.25f, 1.0f) + vec2(0.75f, 0.0f), flAdjustedShadowMapDepthFar));
				
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
				flShadow += texture(u_sShadowMap, (vecAdjustedShadowMapCoords + VogelDiskSample(i, SHADOW_SAMPLES, u_flShadowBlurScale, flPhi)) * vec2(0.25f, 1.0f) + vec2(0.75f, 0.0f), flAdjustedShadowMapDepth));
			
			flShadow *= SHADOW_SAMPLES_INV_F;
		}
		
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