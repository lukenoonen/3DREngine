#include "SharedFile.h"

EFileType UTIL_FileTypeExtensionToEnum( const char *sFileTypeExtension )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)EFileType::i_count; i++)
	{
		if (UTIL_streq( g_sFileTypeExtensions[i], sFileTypeExtension ))
			return (EFileType)i;
	}

	return EFileType::i_invalid;
}

const char *UTIL_FileTypeEnumToExtension( EFileType eFileType )
{
	return g_sFileTypeExtensions[(EBaseEnum)eFileType];
}

EFileType UTIL_FileTypePrePathToEnum( const char *sFileTypePrePath )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)EFileType::i_count; i++)
	{
		if (UTIL_streq( g_sFileTypePrePaths[i], sFileTypePrePath ))
			return (EFileType)i;
	}

	return EFileType::i_invalid;
}

const char *UTIL_FileTypeEnumToPrePath( EFileType eFileType )
{
	return g_sFileTypePrePaths[(EBaseEnum)eFileType];
}