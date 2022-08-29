#ifndef UTIL_H
#define UTIL_H

char			UTIL_CharLower( char cChar );
char			UTIL_CharUpper( char cChar );

unsigned int	UTIL_StringLength( const char *sString );

int				UTIL_StringCompare( const char *sString1, const char *sString2 );
int				UTIL_StringCompare( const char *sString1, const char *sString2, unsigned int uiSize );
int				UTIL_StringCompareLower( const char *sString1, const char *sString2 );
int				UTIL_StringCompareLower( const char *sString1, const char *sString2, unsigned int uiSize );

bool			UTIL_StringEquals( const char *sString1, const char *sString2 );
bool			UTIL_StringEquals( const char *sString1, const char *sString2, unsigned int uiSize );
bool			UTIL_StringEqualsLower( const char *sString1, const char *sString2 );
bool			UTIL_StringEqualsLower( const char *sString1, const char *sString2, unsigned int uiSize );

bool			UTIL_StringIncludes( const char *sString, const char *sIncludes );
bool			UTIL_StringIncludesLower( const char *sString, const char *sIncludes );

void			UTIL_StringCopy( char *sDest, const char *sSource );
void			UTIL_StringCopy( char *sDest, const char *sSource, unsigned int uiSize );

char			*UTIL_StringEdit( const char *sSource );
char			*UTIL_StringEdit( const char *sSource, unsigned int uiSize );

void			UTIL_StringAdd( char *sDest, const char *sSource1, const char *sSource2 );
char			*UTIL_StringAdd( const char *sSource1, const char *sSource2 );
void			UTIL_StringAdd( char *sDest, const char *sSource1, char cSource2 );
char			*UTIL_StringAdd( const char *sSource1, char cSource2 );
void			UTIL_StringAdd( char *sDest, char cSource1, const char *sSource2 );
char			*UTIL_StringAdd( char cSource1, const char *sSource2 );

const char		*UTIL_StringSearch( const char *sString, const char *sSearch );
char			*UTIL_StringSearch( char *sString, const char *sSearch );
const char		*UTIL_StringSearchLower( const char *sString, const char *sSearch );
char			*UTIL_StringSearchLower( char *sString, const char *sSearch );

const char		*UTIL_CharSearch( const char *sString, char cSearch );
char			*UTIL_CharSearch( char *sString, char cSearch );
const char		*UTIL_CharSearch( const char *sString, const char *sSearch );
char			*UTIL_CharSearch( char *sString, const char *sSearch );

const char		*UTIL_CharSearchInverse( const char *sString, char cSearch );
char			*UTIL_CharSearchInverse( char *sString, char cSearch );
const char		*UTIL_CharSearchInverse( const char *sString, const char *sSearch );
char			*UTIL_CharSearchInverse( char *sString, const char *sSearch );

const char		*UTIL_GetExtension( const char *sFileName );

bool			UTIL_StringToBool( bool &bValue, const char *sString );
bool			UTIL_StringToInt( int &iValue, const char *sString );
bool			UTIL_StringToFloat( float &flValue, const char *sString );

char			*UTIL_IntToString( int iValue );

// TODO: clean this up

#define			UTIL_Read( a, b, c ) ((a).read( (char *)(b), c ) ? true : false)
#define			UTIL_Write( a, b, c ) ((a).write( (char *)(b), c ) ? true : false)

#define			UTIL_Min( a, b ) a < b ? a : b
#define			UTIL_Max( a, b ) a > b ? a : b
#define			UTIL_Clamp( a, b, c ) a < b ? b : (a > c ? c : a)

#endif // UTIL_H