#include "Maths.h"

bool UTIL_GetValue( const CTextItem *pTextItem, glm::vec2 &vecValue )
{
	CTextLine *pTextLine;
	if (!UTIL_GetValue( pTextItem, pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::vec3 &vecValue )
{
	CTextLine *pTextLine;
	if (!UTIL_GetValue( pTextItem, pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 ) && pTextLine->GetValue( vecValue.z, 2 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::vec4 &vecValue )
{
	CTextLine *pTextLine;
	if (!UTIL_GetValue( pTextItem, pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 ) && pTextLine->GetValue( vecValue.z, 2 ) && pTextLine->GetValue( vecValue.w, 3 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::ivec2 &vecValue )
{
	CTextLine *pTextLine;
	if (!UTIL_GetValue( pTextItem, pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::ivec3 &vecValue )
{
	CTextLine *pTextLine;
	if (!UTIL_GetValue( pTextItem, pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 ) && pTextLine->GetValue( vecValue.z, 2 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::ivec4 &vecValue )
{
	CTextLine *pTextLine;
	if (!UTIL_GetValue( pTextItem, pTextLine ))
		return false;

	return pTextLine->GetValue( vecValue.x, 0 ) && pTextLine->GetValue( vecValue.y, 1 ) && pTextLine->GetValue( vecValue.z, 2 ) && pTextLine->GetValue( vecValue.w, 3 );
}

bool UTIL_GetValue( const CTextItem *pTextItem, glm::quat &qValue )
{
	glm::vec3 vecValue;
	if (!UTIL_GetValue( pTextItem, vecValue ))
		return false;

	qValue = glm::radians( vecValue );
	return true;
}