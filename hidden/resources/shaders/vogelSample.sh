float InterleavedGradientNoise(vec3 vecFragPos)
{
	vec3 magic = vec3(0.06711056f, 0.00583715f, 52.9829189f);
	return fract(magic.z * fract(dot(fract(vecFragPos.xy * vecFragPos.yz) * 10000.0f, magic.xy)));
}

vec2 VogelDiskSample(int iSampleIndex, int iSamplesCount, float flBlurScale, float flPhi)
{
	float flRadius = sqrt((float(iSampleIndex) + 0.5f) / (float(iSamplesCount) + 0.5f)) * flBlurScale;
	float flTheta = iSampleIndex * 2.39996322972865332f + flPhi;
	return vec2(flRadius * sin(flTheta), flRadius * cos(flTheta));
}