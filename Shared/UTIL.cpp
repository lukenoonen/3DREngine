#include <UTIL.h>
#include <fstream>

char UTIL_clower( char c )
{
	if (c >= 'A' && c <= 'Z')
		return c + 'a' - 'A';

	return c;
}

int UTIL_strlen( const char *sStr )
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

int UTIL_strcmp( const char *sStr1, const char *sStr2, int iSize )
{
	while (iSize-- && *sStr1 == *sStr2)
	{
		sStr1++;
		sStr2++;
	}

	return *sStr1 - *sStr2;
}

int UTIL_strncmp( const char *sStr1, const char *sStr2, int iSize )
{
	while (iSize-- && UTIL_clower( *sStr1 ) == UTIL_clower( *sStr2 ))
	{
		sStr1++;
		sStr2++;
	}

	return UTIL_clower( *sStr1 ) - UTIL_clower( *sStr2 );
}

bool UTIL_strinc( const char *sStr1, const char *sStr2 )
{
	while (*sStr1 && *sStr2 && *sStr1 == *sStr2)
	{
		sStr1++;
		sStr2++;
	}

	return *sStr2 == 0;
}

bool UTIL_strninc( const char *sStr1, const char *sStr2 )
{
	while (*sStr1 && *sStr2 && UTIL_clower( *sStr1 ) == UTIL_clower( *sStr2 ))
	{
		sStr1++;
		sStr2++;
	}

	return *sStr2 == 0;
}

void UTIL_strcpy( char *sDest, const char *sSource )
{
	while (*sDest++ = *sSource++);
}

void UTIL_strncpy( char *sDest, const char *sSource, int iSize )
{
	int iCount = 0;
	while ((iCount++ < iSize) && (*sDest++ = *sSource++));
}

char *UTIL_stredit( const char *sSource )
{
	char *sEditable = new char[UTIL_strlen( sSource ) + 1];
	UTIL_strcpy( sEditable, sSource );
	return sEditable;
}

void UTIL_stradd( char *sDest, const char *sSource1, const char *sSource2 )
{
	while (*sSource1)
		*sDest++ = *sSource1++;
	while (*sSource2)
		*sDest++ = *sSource2++;
	*sDest = 0;
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
			if (*sStr == *sCharCompare++)
				break;
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
			if (*sStr == *sCharCompare++)
				break;
		}
		if (!*sCharCompare)
			return sStr;
		sStr++;
	}
	return NULL;
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

int UTIL_atoi( const char *sStr, int iLen )
{
	bool bNegativeSign = false;
	if (*sStr == '-')
	{
		bNegativeSign = true;
		sStr++;
		iLen--;
	}
	int iOutput = 0;
	while (iLen > 0)
	{
		iOutput *= 10;
		iOutput += *sStr - '0';
		sStr++;
		iLen--;
	}
	return bNegativeSign ? -iOutput : iOutput;
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

float UTIL_atof( const char *sStr, int iLen )
{
	bool bNegativeSign = false;
	if (*sStr == '-')
	{
		bNegativeSign = true;
		sStr++;
		iLen--;
	}
	const char *sDecimal = NULL;
	float fOutput = 0.0f;
	while (iLen > 0)
	{
		if (*sStr == '.')
			sDecimal = sStr;
		fOutput *= 10.0f;
		fOutput += *sStr - '0';
		sStr++;
		iLen--;
	}
	if (sDecimal)
	{
		int iPow = 1;
		for (int i = (int)(sStr - sDecimal); i >= 0; i--) iPow *= 10;

		fOutput /= (float)(iPow);
	}
	return bNegativeSign ? -fOutput : fOutput;
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

	int iLength = 0;
	while (fFile.get() != EOF)
		iLength++;

	fFile.clear();
	fFile.seekg( 0, fFile.beg );

	char *sContents = new char[iLength + 1];
	fFile.read( sContents, iLength );
	sContents[iLength] = '\0';

	fFile.close();

	return sContents;
}