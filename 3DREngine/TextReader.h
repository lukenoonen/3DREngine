#ifndef TEXTREADER_H
#define TEXTREADER_H

// TODO: CLEAN THIS

#include "Global.h"
#include "Handle.h"

class CTextReader;
class CTextBlock;
class CTextLine;
class CTextItem;

class CTextReader
{
public:
	DECLARE_CLASS_NOBASE( CTextReader )

	CTextReader( const char *sText );
	~CTextReader();

	bool Success( void ) const;

	CTextBlock *GetTextBlock( void ) const;

private:
	char *m_sText;
	CTextBlock *m_pTextBlock;
};

class CTextBlock
{
public:
	DECLARE_CLASS_NOBASE( CTextBlock )

	CTextBlock( char *sTextBlock );
	~CTextBlock();

	bool Success( void ) const;

	unsigned int GetTextLineCount( void ) const;
	CTextLine *GetTextLine( unsigned int uiIndex ) const;

	CTextLine *GetTextLine( const char *sKey ) const;

	template <class T> bool GetValue( T &tValue, unsigned int uiIndex, const char *sKey ) const;

private:
	bool m_bSuccess;
	std::vector<CTextLine *> m_pTextLines;
};

class CTextLine
{
public:
	DECLARE_CLASS_NOBASE( CTextLine )

	CTextLine( char *sTextLine );
	~CTextLine();

	bool Success( void ) const;

	unsigned int GetTextItemCount( void ) const;
	CTextItem *GetTextItem( unsigned int uiIndex ) const;

	bool IsKey( const char *sKey ) const;

	template <class T> bool GetValue( T &tValue, unsigned int uiIndex ) const;

private:
	std::vector<CTextItem *> m_pTextItems;
};

class CTextItem
{
public:
	DECLARE_CLASS_NOBASE( CTextItem )

	CTextItem( char *sTextItem, unsigned char ucActiveData );
	~CTextItem();

	bool Success( void ) const;

	// TODO: clean this, finish up implementing for basic data types maybe
	bool GetValue( CTextBlock *&pValue ) const;
	bool GetValue( CTextLine *&pValue ) const;
	bool GetValue( char *&sValue ) const;
	bool GetValue( const char *&sValue ) const;
	bool GetValue( bool &bValue ) const;
	bool GetValue( int &iValue ) const;
	bool GetValue( float &flValue ) const;
	bool GetValue( glm::vec2 &vec2Value ) const;
	bool GetValue( glm::vec3 &vec3Value ) const;
	bool GetValue( glm::vec4 &vec4Value ) const;
	bool GetValue( glm::ivec2 &vec2Value ) const;
	bool GetValue( glm::ivec3 &vec3Value ) const;
	bool GetValue( glm::ivec4 &vec4Value ) const;
	bool GetValue( CBaseHandle &hData ) const;
	template <class T> bool GetValue( CHandle<T> &hData ) const;

private:
	unsigned char m_ucActiveData; // TODO: maybe make this an enum
	union
	{
		CTextBlock *pTextBlock;
		CTextLine *pTextLine;
		const char *sString;
	} m_uData;
};

template <class T> bool CTextBlock::GetValue( T &tValue, unsigned int uiIndex, const char *sKey ) const
{
	CTextLine *pTextLine = GetTextLine( sKey );
	return pTextLine && pTextLine->GetValue( tValue, usIndex );
}

template <class T> bool CTextLine::GetValue( T &tValue, unsigned int uiIndex ) const
{
	CTextItem *pTextItem = GetTextItem( usIndex );
	return pTextItem && pTextItem->GetValue( tValue );
}

bool CTextItem::GetValue( CTextBlock *&pValue ) const
{
	if (m_ucActiveData != 0)
		return false;

	pValue = m_uData.pTextBlock;
	return true;
}

bool CTextItem::GetValue( CTextLine *&pValue ) const
{
	if (m_ucActiveData != 1)
		return false;

	pValue = m_uData.pTextLine;
	return true;
}

bool CTextItem::GetValue( char *&sValue ) const
{
	if (m_ucActiveData != 0)
		return false;

	sValue = UTIL_StringEdit( m_uData.sString );
	return true;
}

bool CTextItem::GetValue( const char *&sValue ) const
{
	if (m_ucActiveData != 0)
		return false;

	sValue = m_uData.sString;
	return true;
}

bool CTextItem::GetValue( bool &bValue ) const
{
	if (m_ucActiveData != 0)
		return false;

	return UTIL_StringToBool( bValue, m_uData.sString );
}

bool CTextItem::GetValue( int &iValue ) const
{
	if (m_ucActiveData != 0)
		return false;

	return UTIL_StringToInt( iValue, m_uData.sString );
}

bool CTextItem::GetValue( float &flValue ) const
{
	if (m_ucActiveData != 0)
		return false;

	return UTIL_StringToFloat( flValue, m_uData.sString );
}

bool CTextItem::GetValue( glm::vec2 &vec2Value ) const
{
	if (m_ucActiveData != 1)
		return false;

	if (!m_uData.pTextLine->GetValue( vec2Value.x, 0 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec2Value.y, 1 ))
		return false;

	return true;
}

bool CTextItem::GetValue( glm::vec3 &vec3Value ) const
{
	if (m_ucActiveData != 1)
		return false;

	if (!m_uData.pTextLine->GetValue( vec3Value.x, 0 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec3Value.y, 1 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec3Value.z, 2 ))
		return false;

	return true;
}

bool CTextItem::GetValue( glm::vec4 &vec4Value ) const
{
	if (m_ucActiveData != 1)
		return false;

	if (!m_uData.pTextLine->GetValue( vec4Value.x, 0 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec4Value.y, 1 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec4Value.z, 2 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec4Value.w, 3 ))
		return false;

	return true;
}

bool CTextItem::GetValue( glm::ivec2 &vec2Value ) const
{
	if (m_ucActiveData != 1)
		return false;

	if (!m_uData.pTextLine->GetValue( vec2Value.x, 0 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec2Value.y, 1 ))
		return false;

	return true;
}

bool CTextItem::GetValue( glm::ivec3 &vec3Value ) const
{
	if (m_ucActiveData != 1)
		return false;

	if (!m_uData.pTextLine->GetValue( vec3Value.x, 0 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec3Value.y, 1 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec3Value.z, 2 ))
		return false;

	return true;
}

bool CTextItem::GetValue( glm::ivec4 &vec4Value ) const
{
	if (m_ucActiveData != 1)
		return false;

	if (!m_uData.pTextLine->GetValue( vec4Value.x, 0 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec4Value.y, 1 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec4Value.z, 2 ))
		return false;

	if (!m_uData.pTextLine->GetValue( vec4Value.w, 3 ))
		return false;

	return true;
}

bool CTextItem::GetValue( CBaseHandle &hData ) const
{
	const char *sName;
	if (!GetValue( sName ))
		return false;

	hData.SetName( sName );
	return true;
}

template <class T> bool CTextItem::GetValue( CHandle<T> &hData ) const
{
	return GetValue( static_cast<CBaseHandle>(hData) );
}

#endif // TEXTREADER_H