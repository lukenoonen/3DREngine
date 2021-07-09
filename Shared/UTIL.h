#ifndef UTIL_H
#define UTIL_H

// String stuff
char		UTIL_clower( char c );
int			UTIL_strlen( const char *sStr );
int			UTIL_strcmp( const char *sStr1, const char *sStr2 );
int			UTIL_strncmp( const char *sStr1, const char *sStr2 );
int			UTIL_strcmp( const char *sStr1, const char *sStr2, int iSize );
int			UTIL_strncmp( const char *sStr1, const char *sStr2, int iSize );
bool		UTIL_strinc( const char *sStr1, const char *sStr2 );
bool		UTIL_strninc( const char *sStr1, const char *sStr2 );
void		UTIL_strcpy( char *sDest, const char *sSource );
void		UTIL_strncpy( char *sDest, const char *sSource, int iSize );
char		*UTIL_stredit( const char *sSource );
void		UTIL_stradd( char *sDest, const char *sSource1, const char *sSource2 );
const char	*UTIL_strstr( const char *sStr1, const char *sStr2 );
char		*UTIL_strstr( char *sStr1, const char *sStr2 );
const char	*UTIL_strnstr( const char *sStr1, const char *sStr2 );
char		*UTIL_strnstr( char *sStr1, const char *sStr2 );
const char	*UTIL_strchr( const char *sStr, const char cChar );
char		*UTIL_strchr( char *sStr, const char cChar );
const char	*UTIL_strchr( const char *sStr, const char *sChar );
char		*UTIL_strchr( char *sStr, const char *sChar );
const char	*UTIL_strchri( const char *sStr, const char cChar );
char		*UTIL_strchri( char *sStr, const char cChar );
const char	*UTIL_strchri( const char *sStr, const char *sChar );
char		*UTIL_strchri( char *sStr, const char *sChar );
const char	*UTIL_extn( const char *sFileName );
int			UTIL_atoi( const char *sStr );
int			UTIL_atoi( const char *sStr, int iLen );
float		UTIL_atof( const char *sStr );
float		UTIL_atof( const char *sStr, int iLen );
int			UTIL_distc( const char *sStr, const char cChar );
char		*UTIL_readf( const char *sFile );

#define		UTIL_Read( a, b, c, d ) (a.read( (char *)b, c * sizeof(d) ))
#define		UTIL_Write( a, b, c, d ) (a.write( (char *)b, c * sizeof(d) ))

#define		UTIL_Min( a, b ) a < b ? a : b
#define		UTIL_Max( a, b ) a > b ? a : b
#define		UTIL_Clamp( a, b, c ) a < b ? b : (a > c ? c : a)

// Sorting stuff
template <class T>
void UTIL_swap( T &t1, T &t2 )
{
	T tTemp = t1;
	t1 = t2;
	t2 = tTemp;
}

#endif // UTIL_H