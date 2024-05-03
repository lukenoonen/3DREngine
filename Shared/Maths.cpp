#include "Maths.h"

glm::quat UTIL_RotationDifference( glm::vec3 vecStart, glm::vec3 vecEnd )
{
	vecStart = glm::normalize( vecStart );
	vecEnd = glm::normalize( vecEnd );

	float flCosTheta = glm::dot( vecStart, vecEnd );
	glm::vec3 rotationAxis;

	if (flCosTheta < -1.0f + 0.001f)
	{
		rotationAxis = glm::cross( g_vec3Up, vecStart );
		if (glm::length( rotationAxis ) < 0.001f)
			rotationAxis = glm::cross( g_vec3Right, vecStart );

		rotationAxis = normalize( rotationAxis );
		return glm::angleAxis( glm::radians( 180.0f ), rotationAxis );
	}

	rotationAxis = glm::cross( vecStart, vecEnd );

	float flS = glm::sqrt( (1.0f + flCosTheta) * 2.0f );
	float flInverse = 1 / flS;

	return glm::quat(
		flS * 0.5f,
		rotationAxis.x * flInverse,
		rotationAxis.y * flInverse,
		rotationAxis.z * flInverse
	);
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::vec2 &vecValue )
{
	CTextLine *pTextLine;
	if (!pTextItem->GetValue( pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::vec3 &vecValue )
{
	CTextLine *pTextLine;
	if (!pTextItem->GetValue( pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 ) && pTextLine->GetValue( vecValue.z, 2 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::vec4 &vecValue )
{
	CTextLine *pTextLine;
	if (!pTextItem->GetValue( pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 ) && pTextLine->GetValue( vecValue.z, 2 ) && pTextLine->GetValue( vecValue.w, 3 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::ivec2 &vecValue )
{
	CTextLine *pTextLine;
	if (!pTextItem->GetValue( pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::ivec3 &vecValue )
{
	CTextLine *pTextLine;
	if (!pTextItem->GetValue( pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 ) && pTextLine->GetValue( vecValue.z, 2 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::ivec4 &vecValue )
{
	CTextLine *pTextLine;
	if (!pTextItem->GetValue( pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 ) && pTextLine->GetValue( vecValue.z, 2 ) && pTextLine->GetValue( vecValue.w, 3 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::quat &qValue )
{
	glm::vec3 vecValue;
	if (!pTextItem->GetValue( vecValue ))
		return false;

	qValue = glm::radians( vecValue );
	return true;
}