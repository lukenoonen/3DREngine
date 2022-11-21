//=============== Copyright Luke Noonen, All rights reserved. ===============//
//
// Purpose: Declares the CFileReader class and its component class, CFile,
// as well as declares and defines UTIL functions used for reading selected 
// data types from a CFile instance
//
//===========================================================================//

#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include <vector>

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

class CFileReader;
class CFile;

//-----------------------------------------------------------------------------
// UTIL function declarations
//-----------------------------------------------------------------------------

template <class T> bool UTIL_Write( CFile *pFile, T &tData );
template <class T> bool UTIL_Write( CFile *pFile, std::vector<T> &tData );

bool UTIL_Write( CFile *pFile, char *&sData );

template <class T> bool UTIL_Read( CFile *pFile, T &tData );
template <class T> bool UTIL_Read( CFile *pFile, std::vector<T> &tData );

bool UTIL_Read( CFile *pFile, char *&sData );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CFileReader
{
public:
	CFileReader();
	~CFileReader();

	bool ReadFile( const char *sFilePath, int iFlags );
	CFile *GetFile( void ) const;

private:
	CFile *m_pFile;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CFile
{
public:
	CFile( const char *sFilePath, int iFlags );
	~CFile();

	bool Success( void ) const;

	bool Buffer( char *&sBuffer );

	template <class T> bool Write( T &tData );
	template <class T> bool Read( T &tData );

	bool WriteBytes( void *pData, unsigned int uiSize );
	bool ReadBytes( void *pData, unsigned int uiSize );

private:
	std::fstream m_fFileStream;
};

//-----------------------------------------------------------------------------
// Purpose: 
// Output: true upon successful write of value of type T, false otherwise
//-----------------------------------------------------------------------------
template <class T> bool CFile::Write( T &tData )
{
	return UTIL_Write( this, tData );
}

//-----------------------------------------------------------------------------
// Purpose:
// Output: true upon successful read of value of type T, false otherwise
//-----------------------------------------------------------------------------
template <class T> bool CFile::Read( T &tData )
{
	return UTIL_Read( this, tData );
}

//-----------------------------------------------------------------------------
// Purpose:
// Output: true upon successful write of value of type T, false otherwise
//-----------------------------------------------------------------------------
template <class T> bool UTIL_Write( CFile *pFile, T &tData )
{
	return pFile->WriteBytes( &tData, sizeof( T ) );
}

//-----------------------------------------------------------------------------
// Purpose:
// Output: true upon successful write for each value of type T, false otherwise
//-----------------------------------------------------------------------------
template <class T> bool UTIL_Write( CFile *pFile, std::vector<T> &tData )
{
	unsigned int uiSize = tData.size();
	if (!UTIL_Write( pFile, uiSize ))
		return false;

	for (unsigned int i = 0; i < uiSize; i++)
	{
		if (!UTIL_Write( pFile, tData[i] ))
			return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose:
// Output: true upon successful read of value of type T, false otherwise
//-----------------------------------------------------------------------------
template <class T> bool UTIL_Read( CFile *pFile, T &tData )
{
	return pFile->ReadBytes( &tData, sizeof( T ) );
}

//-----------------------------------------------------------------------------
// Purpose:
// Output: true upon successful read for each value of type T, false otherwise
//-----------------------------------------------------------------------------
template <class T> bool UTIL_Read( CFile *pFile, std::vector<T> &tData )
{
	unsigned int uiSize;
	if (!UTIL_Read( pFile, uiSize ))
		return false;

	tData.resize( uiSize );

	for (unsigned int i = 0; i < uiSize; i++)
	{
		if (!UTIL_Read( pFile, tData[i] ))
			return false;
	}

	return true;
}

#endif // FILEREADER_H