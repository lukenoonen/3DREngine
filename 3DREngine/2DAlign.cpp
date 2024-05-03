#include "2DAlign.h"

glm::vec2 UTIL_Get2DAlignOffset_ScreenSpace( E2DAlign e2DAlign )
{
	switch (e2DAlign)
	{
	case E2DAlign::t_topleft:
		return glm::vec2( 0.0f, 0.0f );
	case E2DAlign::t_topcenter:
		return glm::vec2( 0.5f, 0.0f );
	case E2DAlign::t_topright:
		return glm::vec2( 1.0f, 0.0f );
	case E2DAlign::t_middleleft:
		return glm::vec2( 0.0f, 0.5f );
	case E2DAlign::t_middlecenter:
		return glm::vec2( 0.5f, 0.5f );
	case E2DAlign::t_middleright:
		return glm::vec2( 1.0f, 0.5f );
	case E2DAlign::t_bottomleft:
		return glm::vec2( 0.0f, 1.0f );
	case E2DAlign::t_bottomcenter:
		return glm::vec2( 0.5f, 1.0f );
	case E2DAlign::t_bottomright:
		return glm::vec2( 1.0f, 1.0f );
	}

	return g_vec2Zero;
}

glm::vec2 UTIL_Get2DAlignOffset_ClipSpace( E2DAlign e2DAlign )
{
	glm::vec2 vec2ScreenSpace = UTIL_Get2DAlignOffset_ScreenSpace( e2DAlign );
	return glm::vec2( 2.0f * vec2ScreenSpace.x - 1.0f, 1.0f - 2.0f * vec2ScreenSpace.y );
}