#include "TextReader.h"
#include "UTIL.h"

CTextReader::CTextReader( const char *sText )
{
	m_sText = UTIL_stredit( sText );
	m_pTextInformation = new CTextInformation( m_sText );
}
CTextReader::~CTextReader()
{
	delete[] m_sText;
	delete m_pTextInformation;
}

CTextInformation *CTextReader::GetTextInformation( void ) const
{
	return m_pTextInformation;
}

CTextInformation::CTextInformation( char *sTextInformation )
{
	m_bSuccess = false;
	char *sChar = sTextInformation;
	while (true)
	{
		sChar = UTIL_strchr( sChar, ";{\"" );
		if (!sChar)
			break;

		if (*sChar == ';')
		{
			*sChar++ = '\0';
			m_pItems.push_back( new CTextItem( sTextInformation ) );
			sTextInformation = sChar;
		}
		else
		{
			switch (*sChar++)
			{
			case '{':
			{
				sChar = UTIL_strchr( sChar, '}' );
				break;
			}
			case '\"':
			{
				sChar = UTIL_strchr( sChar, '\"' );
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

CTextInformation::~CTextInformation()
{
	for (unsigned int i = 0; i < m_pItems.size(); i++)
		delete m_pItems[i];
}

unsigned int CTextInformation::GetTextItemCount( void ) const
{
	return (unsigned int)m_pItems.size();
}

CTextItem *CTextInformation::GetTextItem( unsigned int uiIndex ) const
{
	if (uiIndex >= m_pItems.size())
		return NULL;

	return m_pItems[uiIndex];
}

CTextItem *CTextInformation::GetTextItem( const char *sKey ) const
{
	for (unsigned int i = 0; i < m_pItems.size(); i++)
	{
		CTextItem *pTextItem = m_pItems[i];
		const char *sItemKey = m_pItems[i]->GetKey();
		if (sItemKey && UTIL_strcmp( sKey, sItemKey ) == 0)
			return pTextItem;
	}

	return NULL;
}

CTextTerm *CTextInformation::GetTextTerm( const char *sKey, unsigned int uiIndex ) const
{
	CTextItem *pTextItem = GetTextItem( sKey );
	if (!pTextItem)
		return NULL;

	return pTextItem->GetTextTerm( uiIndex );
}

bool CTextInformation::GetString( const char *sKey, const char *&sOutput ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	sOutput = pTextTerm->GetString();

	if (!sOutput)
		return false;

	return true;
}

bool CTextInformation::GetInt( const char *sKey, int &iOutput ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	if (!pTextTerm->IsIntFormat())
		return false;

	iOutput = pTextTerm->GetInt();
	return true;
}

bool CTextInformation::GetUnsignedInt( const char *sKey, unsigned int &uiOutput ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	if (!pTextTerm->IsUnsignedIntFormat())
		return false;

	uiOutput = pTextTerm->GetUnsignedInt();
	return true;
}

bool CTextInformation::GetBool( const char *sKey, bool &bOutput ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	if (!pTextTerm->IsBoolFormat())
		return false;

	bOutput = pTextTerm->GetBool();
	return true;
}

bool CTextInformation::GetFloat( const char *sKey, float &flOutput ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	if (!pTextTerm->IsFloatFormat())
		return false;

	flOutput = pTextTerm->GetFloat();
	return true;
}

bool CTextInformation::GetVec2( const char *sKey, glm::vec2 &vec2Output ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	if (!pTextTerm->IsVec2Format())
		return false;

	vec2Output = pTextTerm->GetVec2();
	return true;
}

bool CTextInformation::GetVec3( const char *sKey, glm::vec3 &vec3Output ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	if (!pTextTerm->IsVec3Format())
		return false;

	vec3Output = pTextTerm->GetVec3();
	return true;
}

bool CTextInformation::GetVec4( const char *sKey, glm::vec4 &vec4Output ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	if (!pTextTerm->IsVec4Format())
		return false;

	vec4Output = pTextTerm->GetVec4();
	return true;
}

bool CTextInformation::GetIVec2( const char *sKey, glm::ivec2 &vec2Output ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	if (!pTextTerm->IsIVec2Format())
		return false;

	vec2Output = pTextTerm->GetIVec2();
	return true;
}

bool CTextInformation::GetIVec3( const char *sKey, glm::ivec3 &vec3Output ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	if (!pTextTerm->IsIVec3Format())
		return false;

	vec3Output = pTextTerm->GetIVec3();
	return true;
}

bool CTextInformation::GetIVec4( const char *sKey, glm::ivec4 &vec4Output ) const
{
	CTextTerm *pTextTerm = GetTextTerm( sKey, 1 );
	if (!pTextTerm)
		return false;

	if (!pTextTerm->IsIVec4Format())
		return false;

	vec4Output = pTextTerm->GetIVec4();
	return true;
}

bool CTextInformation::IsSuccess( void ) const
{
	return m_bSuccess;
}

CTextItem::CTextItem( char *sTextItem )
{
	while (true)
	{
		if (!sTextItem)
			break;

		if (!*sTextItem)
			break;

		char *sChar = UTIL_strchri( sTextItem, " \t\n" );
		if (!sChar)
			break;

		char *sCharEnd;
		bool bInformation = false;
		switch (*sChar)
		{
		case '{':
		{
			bInformation = true;
			sCharEnd = UTIL_strchr( ++sChar, '}' );
			break;
		}
		case '\"':
		{
			sCharEnd = UTIL_strchr( ++sChar, '\"' );
			break;
		}
		default:
		{
			sCharEnd = UTIL_strchr( sChar, " \t\n" );
			break;
		}
		}

		if (sCharEnd)
			*sCharEnd++ = '\0';

		m_pTerms.push_back( new CTextTerm( sChar, bInformation ) );

		sTextItem = sCharEnd;
	}
}

CTextItem::~CTextItem()
{
	for (unsigned int i = 0; i < m_pTerms.size(); i++)
		delete m_pTerms[i];
}

const char *CTextItem::GetKey( void ) const
{
	return m_pTerms[0]->GetString();
}

CTextTerm *CTextItem::GetTextTerm( unsigned int uiIndex ) const
{
	if (uiIndex >= m_pTerms.size())
		return NULL;

	return m_pTerms[uiIndex];
}

unsigned int CTextItem::GetTextTermCount( void ) const
{
	return (unsigned int)m_pTerms.size();
}

CTextTerm::CTextTerm( char *sTextTerm, bool bInformation )
{
	if (bInformation)
	{
		m_pTextInformation = new CTextInformation( sTextTerm );
		m_sString = NULL;
	}
	else
	{
		m_pTextInformation = NULL;
		m_sString = sTextTerm;
	}
}

CTextTerm::~CTextTerm()
{
	if (m_pTextInformation)
		delete m_pTextInformation;
}

CTextInformation *CTextTerm::GetTextInformation( void ) const
{
	return m_pTextInformation;
}

const char *CTextTerm::GetString( void ) const
{
	return m_sString;
}

bool CTextTerm::IsIntFormat( void ) const
{
	return m_sString != NULL && UTIL_ifmt( m_sString );
}

int CTextTerm::GetInt( void ) const
{
	return UTIL_atoi( m_sString );
}

bool CTextTerm::IsUnsignedIntFormat( void ) const
{
	return m_sString != NULL && UTIL_uifmt( m_sString );
}

unsigned int CTextTerm::GetUnsignedInt( void ) const
{
	return UTIL_atoui( m_sString );
}

bool CTextTerm::IsBoolFormat( void ) const
{
	return m_sString != NULL && (UTIL_streq( "true", m_sString ) || UTIL_streq( "false", m_sString ) || ((m_sString[0] == '0' || m_sString[0] == '1') && m_sString[1] == '\0'));
}

bool CTextTerm::GetBool( void ) const
{
	if (UTIL_streq( "true", m_sString ))
		return true;

	if (UTIL_streq( "false", m_sString ))
		return false;

	return m_sString[0] == '1';
}

bool CTextTerm::IsFloatFormat( void ) const
{
	return m_sString != NULL && UTIL_ffmt( m_sString );
}

float CTextTerm::GetFloat( void ) const
{
	return UTIL_atof( m_sString );
}

bool CTextTerm::IsVec2Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_ffmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_ffmt( sSearchEnd + 1 );
}

glm::vec2 CTextTerm::GetVec2( void ) const
{
	glm::vec2 vec2Output;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec2Output.x = UTIL_atof( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec2Output.y = UTIL_atof( sSearchEnd + 1 );
	return vec2Output;
}

bool CTextTerm::IsVec3Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_ffmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_ffmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_ffmt( sSearchEnd + 1 );
}

glm::vec3 CTextTerm::GetVec3( void ) const
{
	glm::vec3 vec3Output;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec3Output.x = UTIL_atof( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec3Output.y = UTIL_atof( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec3Output.z = UTIL_atof( sSearchEnd + 1 );
	return vec3Output;
}

bool CTextTerm::IsVec4Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_ffmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_ffmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_ffmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_ffmt( sSearchEnd + 1 );
}

glm::vec4 CTextTerm::GetVec4( void ) const
{
	glm::vec4 vec4Output;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec4Output.x = UTIL_atof( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec4Output.y = UTIL_atof( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec4Output.z = UTIL_atof( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec4Output.w = UTIL_atof( sSearchEnd + 1 );
	return vec4Output;
}

bool CTextTerm::IsIVec2Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_ifmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_ifmt( sSearchEnd + 1 );
}

glm::ivec2 CTextTerm::GetIVec2( void ) const
{
	glm::ivec2 vec2Output;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec2Output.x = UTIL_atoi( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec2Output.y = UTIL_atoi( sSearchEnd + 1 );
	return vec2Output;
}

bool CTextTerm::IsIVec3Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_ifmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_ifmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_ifmt( sSearchEnd + 1 );
}

glm::ivec3 CTextTerm::GetIVec3( void ) const
{
	glm::ivec3 vec3Output;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec3Output.x = UTIL_atoi( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec3Output.y = UTIL_atoi( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec3Output.z = UTIL_atoi( sSearchEnd + 1 );
	return vec3Output;
}

bool CTextTerm::IsIVec4Format( void ) const
{
	if (!m_sString)
		return false;

	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_ifmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	if (!UTIL_ifmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ))
		return false;

	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	if (!sSearchEnd)
		return false;

	return UTIL_ifmt( sSearch, (unsigned int)(sSearchEnd - sSearch) ) && UTIL_ifmt( sSearchEnd + 1 );
}

glm::ivec4 CTextTerm::GetIVec4( void ) const
{
	glm::ivec4 vec4Output;
	const char *sSearch = m_sString;
	const char *sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec4Output.x = UTIL_atoi( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec4Output.y = UTIL_atoi( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	sSearch = sSearchEnd + 1;
	sSearchEnd = UTIL_strchr( sSearch, ' ' );
	vec4Output.z = UTIL_atoi( sSearch, (unsigned int)(sSearchEnd - sSearch) );
	vec4Output.w = UTIL_atoi( sSearchEnd + 1 );
	return vec4Output;
}