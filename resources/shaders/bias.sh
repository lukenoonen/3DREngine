float bias(float flNormalDirectionDot)
{
	return max(0.004f * (1.0f - flNormalDirectionDot), 0.001f);
}