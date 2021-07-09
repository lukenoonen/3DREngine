#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Global.h"

class CFileManager
{
public:
	DECLARE_CLASS_NOBASE( CFileManager );

	CFileManager();

	void Load( const char *sFileName );

private:
	bool LoadConfig( const char *sFileName );
};

#endif // FILEMANAGER_H