#ifndef SHAREDGLOBAL_H
#define SHAREDGLOBAL_H

#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <TextReader/TextReader.h>
#include <FileReader/FileReader.h>

#include "Maths.h"
#include "Strings.h"

#define DECLARE_CLASS( thisClass, baseClass )			typedef thisClass ThisClass; \
														typedef baseClass BaseClass;

#define DECLARE_CLASS_NOBASE( thisClass )				typedef thisClass ThisClass;

typedef unsigned char EBaseEnum;

template <class T> const char **UTIL_GetEnumNames( void );

template <class T> T UTIL_EnumNameToValue( const char *sName )
{
	static const char **sNames = UTIL_GetEnumNames<T>();

	for (EBaseEnum i = 0; i < (EBaseEnum)T::i_count; i++)
	{
		if (UTIL_streq( sName, sNames[i] ))
			return (T)i;
	}

	return T::i_invalid;
}

#define DEFINE_ENUM_NAMES( enumname, names ) \
	template <> inline const char **UTIL_GetEnumNames<enumname>( void ) { return names; }

#define DEFINE_ENUM_GETVALUE( enumname )	\
	inline bool UTIL_GetValue( const CTextItem *pTextItem, enumname &eValue ) \
	{ \
		const char *sName; \
		if (!pTextItem->GetValue( sName )) \
			return false; \
		eValue = UTIL_EnumNameToValue<enumname>( sName ); \
		return eValue != enumname::i_invalid; \
	}

#endif // SHAREDGLOBAL_H