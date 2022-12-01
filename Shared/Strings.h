#ifndef STRINGS_H
#define STRINGS_H

// TODO: verify all of these work
#pragma warning(disable:4996)

#include <string>

inline int UTIL_strcmp( const char *str1, const char *str2 )
{
	return strcmp( str1, str2 );
}

inline int UTIL_strncmp( const char *str1, const char *str2, size_t num )
{
	return strncmp( str1, str2, num );
}

inline char *UTIL_strcpy( char *destination, const char *source )
{
	return strcpy( destination, source );
}

inline char *UTIL_strncpy( char *destination, const char *source, size_t num )
{
	return strncpy( destination, source, num );
}

inline char *UTIL_strcat( char *destination, const char *source )
{
	return strcat( destination, source );
}

inline char *UTIL_strncat( char *destination, const char *source, size_t num )
{
	return strncat( destination, source, num );
}

inline bool UTIL_streq( const char *str1, const char *str2 )
{
	return str1 == str2 || strcmp( str1, str2 ) == 0;
}

inline bool UTIL_strneq( const char *str1, const char *str2, size_t num )
{
	return str1 == str2 || strncmp( str1, str2, num ) == 0;
}

inline const char *UTIL_strchr( const char *str1, int character )
{
	return strchr( str1, character );
}

inline char *UTIL_strchr( char *str1, int character )
{
	return strchr( str1, character );
}

inline const char *UTIL_strstr( const char *str1, const char *str2 )
{
	return strstr( str1, str2 );
}

inline char *UTIL_strstr( char *str1, const char *str2 )
{
	return strstr( str1, str2 );
}

inline const char *UTIL_strpbrk( const char *str1, const char *str2 )
{
	return strpbrk( str1, str2 );
}

inline char *UTIL_strpbrk( char *str1, const char *str2 )
{
	return strpbrk( str1, str2 );
}

inline char *UTIL_strskip( char *str1, const char *str2 )
{
	char *out = str1 + strspn( str1, str2 );
	return *out ? out : NULL;
}

inline const char *UTIL_strskip( const char *str1, const char *str2 )
{
	const char *out = str1 + strspn( str1, str2 );
	return *out ? out : NULL;
}

inline const char *UTIL_strchrl( const char *str1, int character )
{
	const char *out = NULL;
	const char *next = str1;
	while (next = strchr( str1, character ))
		out = next++;

	return out;
}

inline char *UTIL_strchrl( char *str1, int character )
{
	char *out = NULL;
	char *next = str1;
	while (next = strchr( str1, character ))
		out = next++;

	return out;
}

inline const char *UTIL_strstrl( const char *str1, const char *str2 )
{
	const char *out = NULL;
	const char *next = str1;
	while (next = strstr( next, str2 ))
		out = next++;

	return out;
}

inline char *UTIL_strstrl( char *str1, const char *str2 )
{
	char *out = NULL;
	char *next = str1;
	while (next = strstr( next, str2 ))
		out = next++;

	return out;
}

inline const char *UTIL_strpbrkl( const char *str1, const char *str2 )
{
	const char *out = NULL;
	const char *next = str1;
	while (next = strpbrk( next, str2 ))
		out = next++;

	return out;
}

inline char *UTIL_strpbrkl( char *str1, const char *str2 )
{
	char *out = NULL;
	char *next = str1;
	while (next = strpbrk( next, str2 ))
		out = next++;

	return out;
}

inline const char *UTIL_strskipl( const char *str1, const char *str2 )
{
	const char *out = NULL;
	const char *next = str1;
	while (next = UTIL_strskip( next, str2 ))
		out = next++;

	return out;
}

inline char *UTIL_strskipl( char *str1, const char *str2 )
{
	char *out = NULL;
	char *next = str1;
	while (next = UTIL_strskip( next, str2 ))
		out = next++;

	return out;
}

inline char *UTIL_strdup( const char *str1 )
{
	size_t num = strlen( str1 );
	char *out = new char[num + 1];
	strncpy( out, str1, num );
	out[num] = '\0';
	return out;
}

inline char *UTIL_strndup( const char *str1, size_t num )
{
	char *out = new char[num + 1];
	strncpy( out, str1, num );
	out[num] = '\0';
	return out;
}

inline char *UTIL_stradd( const char *str1, const char *str2 )
{
	size_t num1 = strlen( str1 );
	size_t num2 = strlen( str2 );
	char *out = new char[num1 + num2 + 1];
	strcpy( out, str1 );
	strcpy( out + num1, str2 );
	return out;
}

inline char *UTIL_strnadd( const char *str1, const char *str2, size_t num ) // TODO: optimise/validate/clean
{
	size_t num1 = strlen( str1 );
	if (num1 > num)
	{
		num1 = num;
		num = 0;
	}
	else
		num -= num1;

	size_t num2 = strlen( str2 );
	if (num2 > num)
		num2 = num;

	char *out = new char[num1 + num2 + 1];
	strncpy( out, str1, num1 );
	strncpy( out + num1, str2, num2 );
	return out;
}

inline char *UTIL_extn( char *str1 )
{
	char *out = strchr( str1, '.' );
	return out ? out + 1 : NULL;
}

#endif // STRINGS_H