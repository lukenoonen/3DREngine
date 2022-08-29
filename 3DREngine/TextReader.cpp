#include "TextReader.h"
#include "UTIL.h"

// TODO: CLEAN THIS

CTextReader::CTextReader( const char *sText )
{
	m_sText = UTIL_StringEdit( sText );
	m_pTextBlock = new CTextBlock( m_sText );
}

CTextReader::~CTextReader()
{
	delete[] m_sText;
	delete m_pTextBlock;
}

bool CTextReader::Success( void ) const
{
	return m_pTextBlock->Success();
}

CTextBlock *CTextReader::GetTextBlock( void ) const
{
	return m_pTextBlock;
}

CTextBlock::CTextBlock( char *sTextBlock )
{
	m_bSuccess = false;

	char *sChar = sTextBlock;
	while (true)
	{
		sChar = UTIL_CharSearch( sChar, ";{[\"" );
		if (!sChar)
		{
			if (UTIL_CharSearchInverse( sTextBlock, " \t\n" ))
				return;
			else
				break;
		}

		if (*sChar == ';')
		{
			*sChar++ = '\0';
			m_pTextLines.push_back( new CTextLine( sTextBlock ) );
			sTextBlock = sChar;
		}
		else
		{
			switch (*sChar++)
			{
			case '{':
			{
				sChar = UTIL_CharSearch( sChar, '}' );
				break;
			}
			case '[':
			{
				sChar = UTIL_CharSearch( sChar, ']' );
				break;
			}
			case '\"':
			{
				sChar = UTIL_CharSearch( sChar, '\"' );
				break;
			}
			}

			if (!sChar)
				return;

			sChar++;
		}
	}

	m_bSuccess = true;
}

CTextBlock::~CTextBlock()
{
	for (unsigned int i = 0; i < m_pTextLines.size(); i++)
		delete m_pTextLines[i];
}

bool CTextBlock::Success( void ) const
{
	if (!m_bSuccess)
		return false;

	for (unsigned int i = 0; i < m_pTextLines.size(); i++)
	{
		if (!m_pTextLines[i]->Success())
			return false;
	}
}

unsigned int CTextBlock::GetTextLineCount( void ) const
{
	return m_pTextLines.size();
}

CTextLine *CTextBlock::GetTextLine( unsigned int uiIndex ) const
{
	if (uiIndex >= m_pTextLines.size())
		return NULL;

	return m_pTextLines[uiIndex];
}

CTextLine *CTextBlock::GetTextLine( const char *sKey ) const
{
	for (unsigned int i = 0; i < m_pTextLines.size(); i++)
	{
		CTextLine *pTextLine = m_pTextLines[i];
		if (pTextLine->IsKey( sKey ))
			return pTextLine;
	}

	return NULL;
}

CTextLine::CTextLine( char *sTextLine )
{
	while (true)
	{
		if (!sTextLine)
			break;

		if (!*sTextLine)
			break;

		char *sChar = UTIL_CharSearchInverse( sTextLine, " \t\n" );
		if (!sChar)
			break;

		char *sCharEnd;
		unsigned char ucActiveData = 2;
		switch (*sChar)
		{
		case '{':
		{
			ucActiveData = 0;
			sCharEnd = UTIL_CharSearch( ++sChar, '}' );
			break;
		}
		case '[':
		{
			ucActiveData = 1;
			sCharEnd = UTIL_CharSearch( ++sChar, ']' );
			break;
		}
		case '\"':
		{
			sCharEnd = UTIL_CharSearch( ++sChar, '\"' );
			break;
		}
		default:
		{
			sCharEnd = UTIL_CharSearch( sChar, " \t\n" );
			break;
		}
		}

		if (sCharEnd)
			*sCharEnd++ = '\0';

		m_pTextItems.push_back( new CTextItem( sChar, ucActiveData ) );

		sTextLine = sCharEnd;
	}
}

CTextLine::~CTextLine()
{
	for (unsigned int i = 0; i < m_pTextItems.size(); i++)
		delete m_pTextItems[i];
}

bool CTextLine::Success( void ) const
{
	for (unsigned int i = 0; i < m_pTextItems.size(); i++)
	{
		if (!m_pTextItems[i]->Success())
			return false;
	}
}

unsigned int CTextLine::GetTextItemCount( void ) const
{
	return m_pTextItems.size();
}

CTextItem *CTextLine::GetTextItem( unsigned int uiIndex ) const
{
	if (uiIndex >= m_pTextItems.size())
		return NULL;

	return m_pTextItems[uiIndex];
}

bool CTextLine::IsKey( const char *sKey ) const
{
	const char *sCompareKey;
	return GetValue( sCompareKey, 0 ) && UTIL_StringEquals( sKey, sCompareKey );
}

CTextItem::CTextItem( char *sTextItem, unsigned char ucActiveData )
{
	m_ucActiveData = ucActiveData;
	switch (m_ucActiveData)
	{
	case 0:
		m_uData.pTextBlock = new CTextBlock( sTextItem );
		break;
	case 1:
		m_uData.pTextLine = new CTextLine( sTextItem );
		break;
	case 2:
		m_uData.sString = sTextItem;
		break;
	}
}

CTextItem::~CTextItem()
{
	switch (m_ucActiveData)
	{
	case 0:
		delete m_uData.pTextBlock;
		break;
	case 1:
		delete m_uData.pTextLine;
		break;
	}
}

bool CTextItem::Success( void ) const
{
	switch (m_ucActiveData)
	{
	case 0:
		return m_uData.pTextBlock->Success();
	case 1:
		return m_uData.pTextLine->Success();
	}

	return true;
}

CTextBlock *CTextItem::GetTextBlock( void ) const
{
	if (m_ucActiveData != 0)
		return NULL;

	return m_uData.pTextBlock;
}

CTextLine *CTextItem::GetTextLine( void ) const
{
	if (m_ucActiveData != 1)
		return NULL;

	return m_uData.pTextLine;
}

/*
bool CTextItem::IsIntFormat( void ) const
{
	return m_sString != NULL && UTIL_IsIntFormat( m_sString );
}

int CTextItem::GetInt( void ) const
{
	return UTIL_StringToInt( m_sString );
}

bool CTextItem::IsBoolFormat( void ) const
{
	return m_sString != NULL && (UTIL_StringEquals( "true", m_sString ) || UTIL_StringEquals( "false", m_sString ) || ((m_sString[0] == '0' || m_sString[0] == '1') && m_sString[1] == '\0'));
}

bool CTextItem::GetBool( void ) const
{
	if (UTIL_StringEquals( "true", m_sString ))
		return true;

	if (UTIL_StringEquals( "false", m_sString ))
		return false;

	return m_sString[0] == '1';
}

bool CTextItem::IsFloatFormat( void ) const
{
	return m_sString != NULL && UTIL_IsFloatFormat( m_sString );
}

float CTextItem::GetFloat( void ) const
{
	return UTIL_StringToFloat( m_sString );
}

bool CTextItem::IsVec2Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_IsFloatFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_IsFloatFormat( sSearchEnd + 1 );
}

glm::vec2 CTextItem::GetVec2( void ) const
{
	glm::vec2 vec2Value;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec2Value.x = UTIL_StringToFloat( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec2Value.y = UTIL_StringToFloat( sSearchEnd + 1 );
	return vec2Value;
}

bool CTextItem::IsVec3Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_IsFloatFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_IsFloatFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_IsFloatFormat( sSearchEnd + 1 );
}

glm::vec3 CTextItem::GetVec3( void ) const
{
	glm::vec3 vec3Value;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec3Value.x = UTIL_StringToFloat( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec3Value.y = UTIL_StringToFloat( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec3Value.z = UTIL_StringToFloat( sSearchEnd + 1 );
	return vec3Value;
}

bool CTextItem::IsVec4Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_IsFloatFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_IsFloatFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_IsFloatFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_IsFloatFormat( sSearchEnd + 1 );
}

glm::vec4 CTextItem::GetVec4( void ) const
{
	glm::vec4 vec4Value;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec4Value.x = UTIL_StringToFloat( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec4Value.y = UTIL_StringToFloat( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec4Value.z = UTIL_StringToFloat( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec4Value.w = UTIL_StringToFloat( sSearchEnd + 1 );
	return vec4Value;
}

bool CTextItem::IsIVec2Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_IsIntFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_IsIntFormat( sSearchEnd + 1 );
}

glm::ivec2 CTextItem::GetIVec2( void ) const
{
	glm::ivec2 vec2Value;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec2Value.x = UTIL_IsIntFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec2Value.y = UTIL_IsIntFormat( sSearchEnd + 1 );
	return vec2Value;
}

bool CTextItem::IsIVec3Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_IsIntFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_IsIntFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_IsIntFormat( sSearchEnd + 1 );
}

glm::ivec3 CTextItem::GetIVec3( void ) const
{
	glm::ivec3 vec3Value;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec3Value.x = UTIL_StringToInt( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec3Value.y = UTIL_StringToInt( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec3Value.z = UTIL_StringToInt( sSearchEnd + 1 );
	return vec3Value;
}

bool CTextItem::IsIVec4Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_IsIntFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_IsIntFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_IsIntFormat( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_IsIntFormat( sSearchEnd + 1 );
}

glm::ivec4 CTextItem::GetIVec4( void ) const
{
	glm::ivec4 vec4Value;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec4Value.x = UTIL_StringToInt( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec4Value.y = UTIL_StringToInt( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_CharSearch( sSearch, ' ' );
	vec4Value.z = UTIL_StringToInt( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec4Value.w = UTIL_StringToInt( sSearchEnd + 1 );
	return vec4Value;
}*/