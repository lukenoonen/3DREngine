float bias(float flNormalDirectionDot)
{
	return max(0.008f * (1.0f - flNormalDirectionDot), 0.002f);
}