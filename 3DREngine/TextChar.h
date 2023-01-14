#ifndef TEXTCHAR_H
#define TEXTCHAR_H

#include "Global.h"

class CTextChar
{
public:

	DECLARE_CLASS_NOBASE( CTextChar )

	CTextChar( unsigned int uiID, const glm::vec2 &vecTextureCoord, const glm::vec2 &vecMaxTextureCoord, const glm::vec2 &vecOffset, const glm::vec2 &vecSize, float flXAdvance );

	unsigned int GetID( void ) const;
	const glm::vec2 &GetTextureCoord( void ) const;
	const glm::vec2 &GetMaxTextureCoord( void ) const;
	const glm::vec2 &GetOffset( void ) const;
	const glm::vec2 &GetSize( void ) const;
	float GetXAdvance( void ) const;

private:
	unsigned int m_uiID;
	glm::vec2 m_vecTextureCoord;
	glm::vec2 m_vecMaxTextureCoord;
	glm::vec2 m_vecOffset;
	glm::vec2 m_vecSize;
	float m_flXAdvance;
};

#endif // TEXTCHAR_H