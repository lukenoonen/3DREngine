#ifndef UTIL_H
#define UTIL_H

char			UTIL_clower( char c );
unsigned int	UTIL_strlen( const char *sStr );
int				UTIL_strcmp( const char *sStr1, const char *sStr2 );
int				UTIL_strncmp( const char *sStr1, const char *sStr2 );
int				UTIL_strcmp( const char *sStr1, const char *sStr2, unsigned int uiSize );
int				UTIL_strncmp( const char *sStr1, const char *sStr2, unsigned int uiSize );
bool			UTIL_streq( const char *sStr1, const char *sStr2 );
bool			UTIL_strneq( const char *sStr1, const char *sStr2 );
bool			UTIL_streq( const char *sStr1, const char *sStr2, unsigned int uiSize );
bool			UTIL_strneq( const char *sStr1, const char *sStr2, unsigned int uiSize );
bool			UTIL_strinc( const char *sStr1, const char *sStr2 );
bool			UTIL_strninc( const char *sStr1, const char *sStr2 );
void			UTIL_strcpy( char *sDest, const char *sSource );
void			UTIL_strncpy( char *sDest, const char *sSource, unsigned int uiSize );
char			*UTIL_stredit( const char *sSource );
char			*UTIL_stredit( const char *sSource, unsigned int uiSize );
void			UTIL_stradd( char *sDest, const char *sSource1, const char *sSource2 );
char			*UTIL_stradd( const char *sSource1, const char *sSource2 );
void			UTIL_stradd( char *sDest, const char *sSource1, char cSource2 );
char			*UTIL_stradd( const char *sSource1, char cSource2 );
void			UTIL_stradd( char *sDest, char cSource1, const char *sSource2 );
char			*UTIL_stradd( char cSource1, const char *sSource2 );
const char		*UTIL_strstr( const char *sStr1, const char *sStr2 );
char			*UTIL_strstr( char *sStr1, const char *sStr2 );
const char		*UTIL_strnstr( const char *sStr1, const char *sStr2 );
char			*UTIL_strnstr( char *sStr1, const char *sStr2 );
const char		*UTIL_strchr( const char *sStr, const char cChar );
char			*UTIL_strchr( char *sStr, const char cChar );
const char		*UTIL_strchr( const char *sStr, const char *sChar );
char			*UTIL_strchr( char *sStr, const char *sChar );
const char		*UTIL_strchri( const char *sStr, const char cChar );
char			*UTIL_strchri( char *sStr, const char cChar );
const char		*UTIL_strchri( const char *sStr, const char *sChar );
char			*UTIL_strchri( char *sStr, const char *sChar );
const char		*UTIL_strchrl( const char *sStr, const char cChar );
char			*UTIL_strchrl( char *sStr, const char cChar );
const char		*UTIL_strchrl( const char *sStr, const char *sChar );
char			*UTIL_strchrl( char *sStr, const char *sChar );
const char		*UTIL_strchril( const char *sStr, const char cChar );
char			*UTIL_strchril( char *sStr, const char cChar );
const char		*UTIL_strchril( const char *sStr, const char *sChar );
char			*UTIL_strchril( char *sStr, const char *sChar );
const char		*UTIL_extn( const char *sFileName );
bool			UTIL_ifmt( const char *sStr );
bool			UTIL_ifmt( const char *sStr, unsigned int uiLen );
int				UTIL_atoi( const char *sStr );
int				UTIL_atoi( const char *sStr, unsigned int uiLen );
bool			UTIL_uifmt( const char *sStr );
bool			UTIL_uifmt( const char *sStr, unsigned int uiLen );
unsigned int	UTIL_atoui( const char *sStr );
unsigned int	UTIL_atoui( const char *sStr, unsigned int uiLen );
bool			UTIL_ffmt( const char *sStr );
bool			UTIL_ffmt( const char *sStr, unsigned int uiLen );
float			UTIL_atof( const char *sStr );
float			UTIL_atof( const char *sStr, unsigned int uiLen );
int				UTIL_distc( const char *sStr, const char cChar );
unsigned int	UTIL_ilen( int iVal );
unsigned int	UTIL_uilen( int uiVal );
char			*UTIL_itoa( int iVal );
char			*UTIL_uitoa( unsigned int uiVal );
char			*UTIL_readf( const char *sFile );

#define			UTIL_Read( a, b, c ) ((a).read( (char *)(b), c ))
#define			UTIL_Write( a, b, c, d ) ((a).write( (char *)(b), (c) * sizeof(d) ))

#define			UTIL_Min( a, b ) a < b ? a : b
#define			UTIL_Max( a, b ) a > b ? a : b
#define			UTIL_Clamp( a, b, c ) a < b ? b : (a > c ? c : a)

template <class T> void UTIL_swap( T &t1, T &t2 )
{
	T tTemp = t1;
	t1 = t2;
	t2 = tTemp;
}

#endif // UTIL_H