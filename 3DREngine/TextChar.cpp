#include "TextChar.h"

CTextChar::CTextChar( unsigned int uiID, const glm::vec2 &vecTextureCoord, const glm::vec2 &vecMaxTextureCoord, const glm::vec2 &vecOffset, const glm::vec2 &vecSize, float flXAdvance )
{
	m_uiID = uiID;
	m_vecTextureCoord = vecTextureCoord;
	m_vecOffset = vecOffset;
	m_vecSize = vecSize;
	m_flXAdvance = flXAdvance;
}

unsigned int CTextChar::GetID( void ) const
{
	return m_uiID;
}

const glm::vec2 &CTextChar::GetTextureCoord( void ) const
{

}

const glm::vec2 &CTextChar::GetMaxTextureCoord( void ) const
{

}

const glm::vec2 &CTextChar::GetOffset( void ) const
{

}

const glm::vec2 &CTextChar::GetSize( void ) const
{

}

float CTextChar::GetXAdvance( void ) const
{

}