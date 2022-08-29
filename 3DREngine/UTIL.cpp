#include "UTIL.h"
#include <fstream>

char UTIL_CharLower( char cChar )
{
	if (cChar >= 'A' && cChar <= 'Z')
		return cChar + 'a' - 'A';

	return cChar;
}

char UTIL_CharUpper( char c )
{
	if (c >= 'a' && c <= 'z')
		return c + 'A' - 'a';

	return c;
}

unsigned int UTIL_StringLength( const char *sString )
{
	const char *sStringSearch = sString;
	while (*sStringSearch) sStringSearch++;
	return (int)(sStringSearch - sString);
}

int UTIL_StringCompare( const char *sString1, const char *sString2 )
{
	while (*sString1 && *sString2 && *sString1 == *sString2)
	{
		sString1++;
		sString2++;
	}

	return *sString1 - *sString2;
}

int UTIL_StringCompare( const char *sString1, const char *sString2, unsigned int uiSize )
{
	while (uiSize-- && *sString1 == *sString2)
	{
		sString1++;
		sString2++;
	}

	return *sString1 - *sString2;
}

int UTIL_StringCompareLower( const char *sString1, const char *sString2 )
{
	while (*sString1 && *sString2 && UTIL_CharLower( *sString1 ) == UTIL_CharLower( *sString2 ))
	{
		sString1++;
		sString2++;
	}

	return UTIL_CharLower( *sString1 ) - UTIL_CharLower( *sString2 );
}

int UTIL_StringCompareLower( const char *sString1, const char *sString2, unsigned int uiSize )
{
	while (uiSize-- && UTIL_CharLower( *sString1 ) == UTIL_CharLower( *sString2 ))
	{
		sString1++;
		sString2++;
	}

	return UTIL_CharLower( *sString1 ) - UTIL_CharLower( *sString2 );
}

bool UTIL_StringEquals( const char *sString1, const char *sString2 )
{
	return sString1 == sString2 || UTIL_StringCompare( sString1, sString2 ) == 0;
}

bool UTIL_StringEquals( const char *sString1, const char *sString2, unsigned int uiSize )
{
	return sString1 == sString2 || UTIL_StringCompare( sString1, sString2, uiSize ) == 0;
}

bool UTIL_StringEqualsLower( const char *sString1, const char *sString2 )
{
	return sString1 == sString2 || UTIL_StringCompareLower( sString1, sString2 ) == 0;
}

bool UTIL_StringEqualsLower( const char *sString1, const char *sString2, unsigned int uiSize )
{
	return sString1 == sString2 || UTIL_StringCompareLower( sString1, sString2, uiSize ) == 0;
}

bool UTIL_StringIncludes( const char *sString, const char *sIncludes )
{
	while (*sString && *sIncludes && *sString == *sIncludes)
	{
		sString++;
		sIncludes++;
	}

	return *sIncludes == '\0';
}

bool UTIL_StringIncludesLower( const char *sString, const char *sIncludes )
{
	while (*sString && *sIncludes && UTIL_CharLower( *sString ) == UTIL_CharLower( *sIncludes ))
	{
		sString++;
		sIncludes++;
	}

	return *sIncludes == '\0';
}

void UTIL_StringCopy( char *sDest, const char *sSource )
{
	while (*sDest++ = *sSource++);
}

void UTIL_StringCopy( char *sDest, const char *sSource, unsigned int uiSize )
{
	while ((uiSize--) && (*sDest++ = *sSource++));
}

char *UTIL_StringEdit( const char *sSource )
{
	char *sEditable = new char[UTIL_StringLength( sSource ) + 1];
	UTIL_StringCopy( sEditable, sSource );
	return sEditable;
}

char *UTIL_StringEdit( const char *sSource, unsigned int uiSize )
{
	char *sEditable = new char[uiSize + 1];
	UTIL_StringCopy( sEditable, sSource, uiSize );
	sEditable[uiSize] = '\0';
	return sEditable;
}

void UTIL_StringAdd( char *sDest, const char *sSource1, const char *sSource2 )
{
	while (*sSource1)
		*sDest++ = *sSource1++;
	while (*sSource2)
		*sDest++ = *sSource2++;
	*sDest = '\0';
}

char *UTIL_StringAdd( const char *sSource1, const char *sSource2 )
{
	char *sDest = new char[UTIL_StringLength( sSource1 ) + UTIL_StringLength( sSource2 ) + 1];
	UTIL_StringAdd( sDest, sSource1, sSource2 );
	return sDest;
}

void UTIL_StringAdd( char *sDest, const char *sSource1, char cSource2 )
{
	while (*sSource1)
		*sDest++ = *sSource1++;
	
	*sDest++ = cSource2;
	*sDest = '\0';
}

char *UTIL_StringAdd( const char *sSource1, char cSource2 )
{
	char *sDest = new char[UTIL_StringLength( sSource1 ) + 2];
	UTIL_StringAdd( sDest, sSource1, cSource2 );
	return sDest;
}

void UTIL_StringAdd( char *sDest, char cSource1, const char *sSource2 )
{
	*sDest++ = cSource1;
	while (*sSource2)
		*sDest++ = *sSource2++;

	*sDest = '\0';
}

char *UTIL_StringAdd( char cSource1, const char *sSource2 )
{
	char *sDest = new char[UTIL_StringLength( sSource2 ) + 2];
	UTIL_StringAdd( sDest, sSource2, sSource2 );
	return sDest;
}

const char *UTIL_StringSearch( const char *sString, const char *sSearch )
{
	while (*sString)
	{
		if (UTIL_StringIncludes( sString, sSearch ))
			return sString;
		sString++;
	}
	return NULL;
}

char *UTIL_StringSearch( char *sString, const char *sSearch )
{
	while (*sString)
	{
		if (UTIL_StringIncludes( sString, sSearch ))
			return sString;
		sString++;
	}
	return NULL;
}

const char *UTIL_StringSearchLower( const char *sString, const char *sSearch )
{
	while (*sString)
	{
		if (UTIL_StringIncludesLower( sString, sSearch ))
			return sString;
		sString++;
	}
	return NULL;
}

char *UTIL_StringSearchLower( char *sString, const char *sSearch )
{
	while (*sString)
	{
		if (UTIL_StringIncludesLower( sString, sSearch ))
			return sString;
		sString++;
	}
	return NULL;
}

const char *UTIL_CharSearch( const char *sString, const char cSearch )
{
	while (*sString)
	{
		if (*sString == cSearch)
			return sString;
		sString++;
	}
	return NULL;
}

char *UTIL_CharSearch( char *sString, const char cSearch )
{
	while (*sString)
	{
		if (*sString == cSearch)
			return sString;
		sString++;
	}
	return NULL;
}

const char *UTIL_CharSearch( const char *sString, const char *sSearch )
{
	while (*sString)
	{
		const char *sSearchCompare = sSearch;
		while (*sSearchCompare)
		{
			if (*sString == *sSearchCompare++)
				return sString;
		}
		sString++;
	}
	return NULL;
}

char *UTIL_CharSearch( char *sString, const char *sSearch )
{
	while (*sString)
	{
		const char *sSearchCompare = sSearch;
		while (*sSearchCompare)
		{
			if (*sString == *sSearchCompare++)
				return sString;
		}
		sString++;
	}
	return NULL;
}

const char *UTIL_CharSearchInverse( const char *sString, const char cSearch )
{
	while (*sString)
	{
		if (*sString != cSearch)
			return sString;
		sString++;
	}
	return NULL;
}

char *UTIL_CharSearchInverse( char *sString, const char cSearch )
{
	while (*sString)
	{
		if (*sString != cSearch)
			return sString;
		sString++;
	}
	return NULL;
}

const char *UTIL_CharSearchInverse( const char *sString, const char *sSearch )
{
	while (*sString)
	{
		const char *sSearchCompare = sSearch;
		while (*sSearchCompare)
		{
			if (*sString == *sSearchCompare)
				break;

			sSearchCompare++;
		}
		if (!*sSearchCompare)
			return sString;
		sString++;
	}
	return NULL;
}

char *UTIL_CharSearchInverse( char *sString, const char *sSearch )
{
	while (*sString)
	{
		const char *sSearchCompare = sSearch;
		while (*sSearchCompare)
		{
			if (*sString == *sSearchCompare)
				break;

			sSearchCompare++;
		}
		if (!*sSearchCompare)
			return sString;
		sString++;
	}
	return NULL;
}

const char *UTIL_GetExtension( const char *sFileName )
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

bool UTIL_StringToBool( bool &bValue, const char *sString )
{
	if (UTIL_StringCompare( sString, "0" ) || UTIL_StringCompare( sString, "false" ))
		bValue = false;
	else if (UTIL_StringCompare( sString, "1" ) || UTIL_StringCompare( sString, "true" ))
		bValue = true;
	else
		return false;

	return true;
}

bool UTIL_StringToInt( int &iValue, const char *sString )
{
	bool bNegativeSign = false;
	if (*sString == '-')
	{
		bNegativeSign = true;
		sString++;
	}
	iValue = 0;
	while (*sString)
	{
		if (*sString > '9' || *sString < '0')
			return false;

		iValue *= 10;
		iValue += *sString - '0';
		sString++;
	}

	if (bNegativeSign)
		iValue = -iValue;

	return true;
}

bool UTIL_StringToFloat( float &flValue, const char *sString )
{
	bool bNegativeSign = false;
	if (*sString == '-')
	{
		bNegativeSign = true;
		sString++;
	}
	const char *sDecimal = NULL;
	flValue = 0.0f;
	while (*sString)
	{
		if (*sString == '.')
		{
			if (!sDecimal)
				sDecimal = sString;
			else
				return false;
		}
		else
		{
			if (*sString > '9' || *sString < '0')
				return false;

			flValue *= 10.0f;
			flValue += *sString - '0';
		}
		sString++;
	}
	if (sDecimal)
	{
		int iPow = 1;
		for (int i = (int)(sString - sDecimal - 2); i >= 0; i--) iPow *= 10;
		flValue /= (float)(iPow);
	}

	if (bNegativeSign)
		flValue = -flValue;

	return true;
}

char *UTIL_IntToString( int iValue )
{
	int iValueCopy = iValue;
	unsigned int uiSize = iValueCopy < 0 ? 2 : 1;
	while (iValueCopy /= 10) uiSize++;

	char *sString = new char[uiSize];

	if (iValue < 0)
	{
		sString[0] = '-';
		iValue *= -1;
	}

	unsigned int uiIndex = uiSize - 1;

	do
	{
		sString[uiIndex--] = '0' + iValue % 10;
	} while (iValue /= 10);

	return sString;
}