#ifndef TEXTREADER_H
#define TEXTREADER_H

#include "SharedGlobal.h"

class CTextReader;
class CTextInformation;
class CTextItem;
class CTextTerm;

class CTextReader
{
public:
	CTextReader( const char *sText );
	~CTextReader();

	CTextInformation *GetTextInformation( void ) const;

private:
	char *m_sText;
	CTextInformation *m_pTextInformation;
};

class CTextInformation
{
public:
	CTextInformation( char *sTextInformation );
	~CTextInformation();

	unsigned int GetTextItemCount( void ) const;
	CTextItem *GetTextItem( unsigned int uiIndex ) const;
	CTextItem *GetTextItem( const char *sKey ) const;
	CTextTerm *GetTextTerm( const char *sKey, unsigned int uiIndex ) const;

	bool GetString( const char *sKey, const char *&sOutput ) const;
	bool GetInt( const char *sKey, int &iOutput ) const;
	bool GetUnsignedInt( const char *sKey, unsigned int &uiOutput ) const;
	bool GetBool( const char *sKey, bool &bOutput ) const;
	bool GetFloat( const char *sKey, float &flOutput ) const;
	bool GetVec2( const char *sKey, glm::vec2 &vec2Output ) const;
	bool GetVec3( const char *sKey, glm::vec3 &vec3Output ) const;
	bool GetVec4( const char *sKey, glm::vec4 &vec4Output ) const;
	bool GetIVec2( const char *sKey, glm::ivec2 &vec2Output ) const;
	bool GetIVec3( const char *sKey, glm::ivec3 &vec3Output ) const;
	bool GetIVec4( const char *sKey, glm::ivec4 &vec4Output ) const;

	bool IsSuccess( void ) const;

private:
	bool m_bSuccess;
	std::vector<CTextItem *> m_pItems;
};

class CTextItem
{
public:
	CTextItem( char *sTextItem );
	~CTextItem();

	const char *GetKey( void ) const;
	CTextTerm *GetTextTerm( unsigned int uiIndex ) const;
	unsigned int GetTextTermCount( void ) const;

private:
	std::vector<CTextTerm *> m_pTerms;
};

class CTextTerm
{
public:
	CTextTerm( char *sTextTerm, bool bInformation );
	~CTextTerm();

	CTextInformation *GetTextInformation( void ) const;

	const char *GetString( void ) const;
	bool IsIntFormat( void ) const;
	int GetInt( void ) const;
	bool IsUnsignedIntFormat( void ) const;
	unsigned int GetUnsignedInt( void ) const;
	bool IsBoolFormat( void ) const;
	bool GetBool( void ) const;
	bool IsFloatFormat( void ) const;
	float GetFloat( void ) const;
	bool IsVec2Format( void ) const;
	glm::vec2 GetVec2( void ) const;
	bool IsVec3Format( void ) const;
	glm::vec3 GetVec3( void ) const;
	bool IsVec4Format( void ) const;
	glm::vec4 GetVec4( void ) const;
	bool IsIVec2Format( void ) const;
	glm::ivec2 GetIVec2( void ) const;
	bool IsIVec3Format( void ) const;
	glm::ivec3 GetIVec3( void ) const;
	bool IsIVec4Format( void ) const;
	glm::ivec4 GetIVec4( void ) const;

private:
	CTextInformation *m_pTextInformation;
	const char *m_sString;
};

#endif // TEXTREADER_H