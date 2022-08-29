#include "UTIL.h"
#include <fstream>

char UTIL_clower( char c )
{
	if (c >= 'A' && c <= 'Z')
		return c + 'a' - 'A';

	return c;
}

unsigned int UTIL_strlen( const char *sStr )
{
	const char *sStrSearch = sStr;
	while (*sStrSearch) sStrSearch++;
	return (int)(sStrSearch - sStr);
}

int UTIL_strcmp( const char *sStr1, const char *sStr2 )
{
	while (*sStr1 && *sStr2 && *sStr1 == *sStr2)
	{
		sStr1++;
		sStr2++;
	}

	return *sStr1 - *sStr2;
}

int UTIL_strncmp( const char *sStr1, const char *sStr2 )
{
	while (*sStr1 && *sStr2 && UTIL_clower( *sStr1 ) == UTIL_clower( *sStr2 ))
	{
		sStr1++;
		sStr2++;
	}

	return UTIL_clower( *sStr1 ) - UTIL_clower( *sStr2 );
}

int UTIL_strcmp( const char *sStr1, const char *sStr2, unsigned int uiSize )
{
	while (uiSize-- && *sStr1 == *sStr2)
	{
		sStr1++;
		sStr2++;
	}

	return *sStr1 - *sStr2;
}

int UTIL_strncmp( const char *sStr1, const char *sStr2, unsigned int uiSize )
{
	while (uiSize-- && UTIL_clower( *sStr1 ) == UTIL_clower( *sStr2 ))
	{
		sStr1++;
		sStr2++;
	}

	return UTIL_clower( *sStr1 ) - UTIL_clower( *sStr2 );
}

bool UTIL_streq( const char *sStr1, const char *sStr2 )
{
	return UTIL_strcmp( sStr1, sStr2 ) == 0;
}

bool UTIL_strneq( const char *sStr1, const char *sStr2 )
{
	return UTIL_strncmp( sStr1, sStr2 ) == 0;
}

bool UTIL_streq( const char *sStr1, const char *sStr2, unsigned int uiSize )
{
	return UTIL_strcmp( sStr1, sStr2, uiSize ) == 0;
}

bool UTIL_strneq( const char *sStr1, const char *sStr2, unsigned int uiSize )
{
	return UTIL_strncmp( sStr1, sStr2, uiSize ) == 0;
}

bool UTIL_strinc( const char *sStr1, const char *sStr2 )
{
	while (*sStr1 && *sStr2 && *sStr1 == *sStr2)
	{
		sStr1++;
		sStr2++;
	}

	return *sStr2 == '\0';
}

bool UTIL_strninc( const char *sStr1, const char *sStr2 )
{
	while (*sStr1 && *sStr2 && UTIL_clower( *sStr1 ) == UTIL_clower( *sStr2 ))
	{
		sStr1++;
		sStr2++;
	}

	return *sStr2 == '\0';
}

void UTIL_strcpy( char *sDest, const char *sSource )
{
	while (*sDest++ = *sSource++);
}

void UTIL_strncpy( char *sDest, const char *sSource, unsigned int uiSize )
{
	unsigned int uiCount = 0;
	while ((uiCount++ < uiSize) && (*sDest++ = *sSource++));
}

char *UTIL_stredit( const char *sSource )
{
	char *sEditable = new char[UTIL_strlen( sSource ) + 1];
	UTIL_strcpy( sEditable, sSource );
	return sEditable;
}

char *UTIL_stredit( const char *sSource, unsigned int uiSize )
{
	char *sEditable = new char[uiSize + 1];
	UTIL_strncpy( sEditable, sSource, uiSize );
	sEditable[uiSize] = '\0';
	return sEditable;
}

void UTIL_stradd( char *sDest, const char *sSource1, const char *sSource2 )
{
	while (*sSource1)
		*sDest++ = *sSource1++;
	while (*sSource2)
		*sDest++ = *sSource2++;
	*sDest = '\0';
}

char *UTIL_stradd( const char *sSource1, const char *sSource2 )
{
	char *sDest = new char[UTIL_strlen( sSource1 ) + UTIL_strlen( sSource2 ) + 1];
	UTIL_stradd( sDest, sSource1, sSource2 );
	return sDest;
}

void UTIL_stradd( char *sDest, const char *sSource1, char cSource2 )
{
	while (*sSource1)
		*sDest++ = *sSource1++;
	
	*sDest++ = cSource2;
	*sDest = '\0';
}

char *UTIL_stradd( const char *sSource1, char cSource2 )
{
	char *sDest = new char[UTIL_strlen( sSource1 ) + 2];
	UTIL_stradd( sDest, sSource1, cSource2 );
	return sDest;
}

void UTIL_stradd( char *sDest, char cSource1, const char *sSource2 )
{
	*sDest++ = cSource1;
	while (*sSource2)
		*sDest++ = *sSource2++;

	*sDest = '\0';
}

char *UTIL_stradd( char cSource1, const char *sSource2 )
{
	char *sDest = new char[UTIL_strlen( sSource2 ) + 2];
	UTIL_stradd( sDest, sSource2, sSource2 );
	return sDest;
}

const char *UTIL_strstr( const char *sStr1, const char *sStr2 )
{
	while (*sStr1)
	{
		if (UTIL_strinc( sStr1, sStr2 ))
			return sStr1;
		sStr1++;
	}
	return NULL;
}

char *UTIL_strstr( char *sStr1, const char *sStr2 )
{
	while (*sStr1)
	{
		if (UTIL_strinc( sStr1, sStr2 ))
			return sStr1;
		sStr1++;
	}
	return NULL;
}

const char *UTIL_strnstr( const char *sStr1, const char *sStr2 )
{
	while (*sStr1)
	{
		if (UTIL_strninc( sStr1, sStr2 ))
			return sStr1;
		sStr1++;
	}
	return NULL;
}

char *UTIL_strnstr( char *sStr1, const char *sStr2 )
{
	while (*sStr1)
	{
		if (UTIL_strninc( sStr1, sStr2 ))
			return sStr1;
		sStr1++;
	}
	return NULL;
}

const char *UTIL_strchr( const char *sStr, const char cChar )
{
	while (*sStr)
	{
		if (*sStr == cChar)
			return sStr;
		sStr++;
	}
	return NULL;
}

char *UTIL_strchr( char *sStr, const char cChar )
{
	while (*sStr)
	{
		if (*sStr == cChar)
			return sStr;
		sStr++;
	}
	return NULL;
}

const char *UTIL_strchr( const char *sStr, const char *sChar )
{
	while (*sStr)
	{
		const char *sCharCompare = sChar;
		while (*sCharCompare)
		{
			if (*sStr == *sCharCompare++)
				return sStr;
		}
		sStr++;
	}
	return NULL;
}

char *UTIL_strchr( char *sStr, const char *sChar )
{
	while (*sStr)
	{
		const char *sCharCompare = sChar;
		while (*sCharCompare)
		{
			if (*sStr == *sCharCompare++)
				return sStr;
		}
		sStr++;
	}
	return NULL;
}

const char *UTIL_strchri( const char *sStr, const char cChar )
{
	while (*sStr)
	{
		if (*sStr != cChar)
			return sStr;
		sStr++;
	}
	return NULL;
}

char *UTIL_strchri( char *sStr, const char cChar )
{
	while (*sStr)
	{
		if (*sStr != cChar)
			return sStr;
		sStr++;
	}
	return NULL;
}

const char *UTIL_strchri( const char *sStr, const char *sChar )
{
	while (*sStr)
	{
		const char *sCharCompare = sChar;
		while (*sCharCompare)
		{
			if (*sStr == *sCharCompare)
				break;

			sCharCompare++;
		}
		if (!*sCharCompare)
			return sStr;
		sStr++;
	}
	return NULL;
}

char *UTIL_strchri( char *sStr, const char *sChar )
{
	while (*sStr)
	{
		const char *sCharCompare = sChar;
		while (*sCharCompare)
		{
			if (*sStr == *sCharCompare)
				break;

			sCharCompare++;
		}
		if (!*sCharCompare)
			return sStr;
		sStr++;
	}
	return NULL;
}

const char *UTIL_strchrl( const char *sStr, const char cChar )
{
	const char *sOutput = NULL;
	while (sStr = UTIL_strchr( sStr, cChar )) sOutput = sStr++;
	return sOutput;
}

char *UTIL_strchrl( char *sStr, const char cChar )
{
	char *sOutput = NULL;
	while (sStr = UTIL_strchr( sStr, cChar )) sOutput = sStr++;
	return sOutput;
}

const char *UTIL_strchrl( const char *sStr, const char *sChar )
{
	const char *sOutput = NULL;
	while (sStr = UTIL_strchr( sStr, sChar )) sOutput = sStr++;
	return sOutput;
}

char *UTIL_strchrl( char *sStr, const char *sChar )
{
	char *sOutput = NULL;
	while (sStr = UTIL_strchr( sStr, sChar )) sOutput = sStr++;
	return sOutput;
}

const char *UTIL_strchril( const char *sStr, const char cChar )
{
	const char *sOutput = NULL;
	while (sStr = UTIL_strchri( sStr, cChar )) sOutput = sStr++;
	return sOutput;
}

char *UTIL_strchril( char *sStr, const char cChar )
{
	char *sOutput = NULL;
	while (sStr = UTIL_strchri( sStr, cChar )) sOutput = sStr++;
	return sOutput;
}

const char *UTIL_strchril( const char *sStr, const char *sChar )
{
	const char *sOutput = NULL;
	while (sStr = UTIL_strchri( sStr, sChar )) sOutput = sStr++;
	return sOutput;
}

char *UTIL_strchril( char *sStr, const char *sChar )
{
	char *sOutput = NULL;
	while (sStr = UTIL_strchri( sStr, sChar )) sOutput = sStr++;
	return sOutput;
}

const char *UTIL_extn( const char *sFileName )
{
	const char *sExtension = NULL;
	while (*sFileName)
	{
		if (*sFileName == '.')
			sExtension = sFileName + 1;

		sFileName++;
	}

	return sExtension;
}

bool UTIL_ifmt( const char *sStr )
{
	if (*sStr == '-')
		sStr++;

	while (*sStr)
	{
		if (*sStr > '9' || *sStr < '0')
			return false;

		sStr++;
	}

	return true;
}

bool UTIL_ifmt( const char *sStr, unsigned int uiLen )
{
	if (*sStr == '-')
	{
		sStr++;
		uiLen--;
	}

	while (uiLen > 0)
	{
		if (*sStr > '9' || *sStr < '0')
			return false;

		sStr++;
		uiLen--;
	}

	return true;
}

int UTIL_atoi( const char *sStr )
{
	bool bNegativeSign = false;
	if (*sStr == '-')
	{
		bNegativeSign = true;
		sStr++;
	}
	int iOutput = 0;
	while (*sStr)
	{
		iOutput *= 10;
		iOutput += *sStr - '0';
		sStr++;
	}
	return bNegativeSign ? -iOutput : iOutput;
}

int UTIL_atoi( const char *sStr, unsigned int uiLen )
{
	bool bNegativeSign = false;
	if (*sStr == '-')
	{
		bNegativeSign = true;
		sStr++;
		uiLen--;
	}
	int iOutput = 0;
	while (uiLen > 0)
	{
		iOutput *= 10;
		iOutput += *sStr - '0';
		sStr++;
		uiLen--;
	}
	return bNegativeSign ? -iOutput : iOutput;
}

bool UTIL_uifmt( const char *sStr )
{
	while (*sStr)
	{
		if (*sStr > '9' || *sStr < '0')
			return false;

		sStr++;
	}

	return true;
}

bool UTIL_uifmt( const char *sStr, unsigned int uiLen )
{
	while (uiLen > 0)
	{
		if (*sStr > '9' || *sStr < '0')
			return false;

		sStr++;
		uiLen--;
	}

	return true;
}

unsigned int UTIL_atoui( const char *sStr )
{
	unsigned int uiOutput = 0;
	while (*sStr)
	{
		uiOutput *= 10;
		uiOutput += *sStr - '0';
		sStr++;
	}
	return uiOutput;
}

unsigned int UTIL_atoui( const char *sStr, unsigned int uiLen )
{
	unsigned int uiOutput = 0;
	while (uiLen > 0)
	{
		uiOutput *= 10;
		uiOutput += *sStr - '0';
		sStr++;
		uiLen--;
	}
	return uiOutput;
}

bool UTIL_ffmt( const char *sStr )
{
	if (*sStr == '-')
		sStr++;

	bool bDecimal = false;
	while (*sStr)
	{
		if (!bDecimal && *sStr == '.')
		{
			bDecimal = true;
		}
		else
		{
			if (*sStr > '9' || *sStr < '0')
				return false;
		}

		sStr++;
	}

	return true;
}

bool UTIL_ffmt( const char *sStr, unsigned int uiLen )
{
	if (*sStr == '-')
	{
		sStr++;
		uiLen--;
	}

	bool bDecimal = false;
	while (uiLen > 0)
	{
		if (*sStr == '.')
		{
			if (!bDecimal)
				bDecimal = true;
			else
				return false;
		}
		else
		{
			if (*sStr > '9' || *sStr < '0')
				return false;
		}

		sStr++;
		uiLen--;
	}

	return true;
}

float UTIL_atof( const char *sStr )
{
	bool bNegativeSign = false;
	if (*sStr == '-')
	{
		bNegativeSign = true;
		sStr++;
	}
	const char *sDecimal = NULL;
	float fOutput = 0.0f;
	while (*sStr)
	{
		if (*sStr == '.')
		{
			sDecimal = sStr;
		}
		else
		{
			fOutput *= 10.0f;
			fOutput += *sStr - '0';
		}
		sStr++;
	}
	if (sDecimal)
	{
		int iPow = 1;
		for (int i = (int)(sStr - sDecimal - 2); i >= 0; i--) iPow *= 10;
		fOutput /= (float)(iPow);
	}
	return bNegativeSign ? -fOutput : fOutput;
}

float UTIL_atof( const char *sStr, unsigned int uiLen )
{
	bool bNegativeSign = false;
	if (*sStr == '-')
	{
		bNegativeSign = true;
		sStr++;
		uiLen--;
	}
	const char *sDecimal = NULL;
	float fOutput = 0.0f;
	while (uiLen > 0)
	{
		if (*sStr == '.')
			sDecimal = sStr;
		fOutput *= 10.0f;
		fOutput += *sStr - '0';
		sStr++;
		uiLen--;
	}
	if (sDecimal)
	{
		int iPow = 1;
		for (int i = (int)(sStr - sDecimal); i >= 0; i--) iPow *= 10;

		fOutput /= (float)(iPow);
	}
	return bNegativeSign ? -fOutput : fOutput;
}

unsigned int UTIL_ilen( int iVal )
{
	unsigned int uiLen = iVal < 0 ? 2 : 1;
	while (iVal /= 10) uiLen++;
	return uiLen;
}

unsigned int UTIL_uilen( int uiVal )
{
	unsigned int uiLen = 1;
	while (uiVal /= 10) uiLen++;
	return uiLen;
}

char *UTIL_itoa( int iVal )
{
	unsigned int uiLen = UTIL_ilen( iVal );
	char *sStr = new char[uiLen];

	if (iVal < 0)
	{
		sStr[0] = '-';
		iVal *= -1;
	}

	unsigned int uiIndex = uiLen - 1;

	do
	{
		sStr[uiIndex--] = '0' + iVal % 10;
	} while (iVal /= 10);

	return sStr;
}

char *UTIL_uitoa( unsigned int uiVal )
{
	unsigned int uiLen = UTIL_uilen( uiVal );
	char *sStr = new char[uiLen];

	unsigned int uiIndex = uiLen - 1;

	do
	{
		sStr[uiIndex--] = '0' + uiVal % 10;
	} while (uiVal /= 10);

	return sStr;
}

int UTIL_distc( const char *sStr, const char cChar )
{
	const char *sStrSearch = sStr;
	while (*sStrSearch != cChar)
	{
		if (!*sStrSearch)
			return -1;

		sStrSearch++;
	}
	return (int)(sStrSearch - sStr + 1);
}

char *UTIL_readf( const char *sFile )
{
	std::fstream fFile( sFile, std::ios::in );

	if (!fFile.is_open())
		return NULL;

	unsigned int uiLength = 0;
	while (fFile.get() != EOF)
		uiLength++;

	fFile.clear();
	fFile.seekg( 0, fFile.beg );

	char *sContents = new char[uiLength + 1];
	fFile.read( sContents, uiLength );
	sContents[uiLength] = '\0';

	fFile.close();

	return sContents;
}