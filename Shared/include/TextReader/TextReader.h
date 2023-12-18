//=============== Copyright Luke Noonen, All rights reserved. ===============//
//
// Purpose: Declares the CTextReader class and its component classes,
// CTextBlock, CTextLine, and CTextItem, as well as UTIL functions
// used for extracting selected data types from a CTextItem instance
//
//===========================================================================//

#ifndef TEXTREADER_H
#define TEXTREADER_H

#include <vector>
#include <string>

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

class CTextReader;
class CTextBlock;
class CTextLine;
class CTextItem;

//-----------------------------------------------------------------------------
// UTIL function declarations
//-----------------------------------------------------------------------------

bool UTIL_GetValue( const CTextItem *pTextItem, CTextBlock *&pValue );
bool UTIL_GetValue( const CTextItem *pTextItem, CTextLine *&pValue );
bool UTIL_GetValue( const CTextItem *pTextItem, char *&sValue );
bool UTIL_GetValue( const CTextItem *pTextItem, const char *&sValue );
bool UTIL_GetValue( const CTextItem *pTextItem, bool &bValue );
bool UTIL_GetValue( const CTextItem *pTextItem, int &iValue );
bool UTIL_GetValue( const CTextItem *pTextItem, unsigned int &uiValue );
bool UTIL_GetValue( const CTextItem *pTextItem, char &cValue );
bool UTIL_GetValue( const CTextItem *pTextItem, unsigned char &ucValue );
bool UTIL_GetValue( const CTextItem *pTextItem, float &fValue );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTextReader
{
public:
	CTextReader();
	~CTextReader();

	bool ReadText( const char *sText );
	CTextBlock *GetTextBlock( void ) const;

private:
	// TODO: change to go through original const char *sText and modified char *sText so there's no conflict with overwriting expected text
	bool ProcessText( char *sText );

private:
	char *m_sText;
	CTextBlock *m_pTextBlock;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTextBlock
{
public:
	CTextBlock();

	~CTextBlock();

	void AddTextLine( CTextLine *pTextLine );

	unsigned int GetTextLineCount( void ) const;
	CTextLine *GetTextLine( unsigned int uiIndex ) const;

	CTextLine *GetTextLine( const char *sKey ) const;

	template <class T> bool GetValue( T &tValue, unsigned int uiIndex, const char *sKey ) const;

private:
	std::vector<CTextLine *> m_pTextLines;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTextLine
{
public:
	CTextLine();

	~CTextLine();

	void AddTextItem( CTextItem *pTextItem );

	unsigned int GetTextItemCount( void ) const;
	CTextItem *GetTextItem( unsigned int uiIndex ) const;

	bool IsKey( const char *sKey ) const;

	template <class T> bool GetValue( T &tValue, unsigned int uiIndex ) const;

private:
	std::vector<CTextItem *> m_pTextItems;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTextItem
{
public:
	CTextItem( CTextBlock *pTextBlock );
	CTextItem( CTextLine *pTextLine );
	CTextItem( const char *sString );

	//CTextItem( char *&sTextItem, char &cResult, char cExpecting );

	~CTextItem();

	CTextBlock *GetTextBlock( void ) const;
	CTextLine *GetTextLine( void ) const;
	const char *GetString( void ) const;

	template <class T> bool GetValue( T &tValue ) const;

private:
	unsigned char m_ucActiveData; // TODO: maybe make this an enum
	union
	{
		CTextBlock *pTextBlock;
		CTextLine *pTextLine;
		const char *sString;
	} m_uData;
};

//-----------------------------------------------------------------------------
// Purpose:
// Output: true upon successful read of value of type T of the given index
//         of the given key, false otherwise
//-----------------------------------------------------------------------------
template <class T> bool CTextBlock::GetValue( T &tValue, unsigned int uiIndex, const char *sKey ) const
{
	CTextLine *pTextLine = GetTextLine( sKey );
	return pTextLine && pTextLine->GetValue( tValue, uiIndex );
}

//-----------------------------------------------------------------------------
// Purpose:
// Output: true upon successful read of value of type T of the given index,
//         false otherwise
//-----------------------------------------------------------------------------
template <class T> bool CTextLine::GetValue( T &tValue, unsigned int uiIndex ) const
{
	CTextItem *pTextItem = GetTextItem( uiIndex );
	return pTextItem && pTextItem->GetValue( tValue );
}

//-----------------------------------------------------------------------------
// Purpose:
// Output: true upon successful read of value of type T, false otherwise
//-----------------------------------------------------------------------------
template <class T> bool CTextItem::GetValue( T &tValue ) const
{
	return UTIL_GetValue( this, tValue );
}

#endif // TEXTREADER_H