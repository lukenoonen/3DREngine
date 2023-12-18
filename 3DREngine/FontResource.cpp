#include "FontResource.h"
#include "RenderManager.h"
#include "ResourceManager.h"

CFontResource::CFontResource( SFontResourceData &sData, const char *sName ) : BaseClass( sName )
{
	m_uiFontSize = sData.uiFontSize;

	m_vec2BitmapSize = sData.vec2BitmapSize;

	glGenTextures( 1, &m_glBitmapID );
	glBindTexture( GL_TEXTURE_2D, m_glBitmapID );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_R8, m_vec2BitmapSize.x, m_vec2BitmapSize.y, 0, GL_RED, GL_UNSIGNED_BYTE, sData.pBitmapData );
	glGenerateMipmap( GL_TEXTURE_2D );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); // TODO: Verify this works
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	memcpy( m_sChars, sData.sChars, sizeof( SChar ) * FONT_CHAR_SIZE ); // TODO: Verify this works
}

CFontResource::~CFontResource()
{
	pRenderManager->UnbindTexture( m_glBitmapID );
	glDeleteTextures( 1, &m_glBitmapID );
}

unsigned int CFontResource::GetFontSize( void ) const
{
	return m_uiFontSize;
}

const glm::ivec2 &CFontResource::GetBitmapSize( void ) const
{
	return m_vec2BitmapSize;
}

int CFontResource::BindBitmap( void ) const
{
	return pRenderManager->BindTexture( m_glBitmapID, GL_TEXTURE_2D );
}

const SChar &CFontResource::GetChar( char cChar ) const
{
	return m_sChars[cChar - FONT_CHAR_START];
}

bool UTIL_Write( CFile *pFile, CFontResource *&pData )
{
	const char *sName = pData->GetName();
	return pFile->Write( sName );
}

bool UTIL_Read( CFile *pFile, CFontResource *&pData )
{
	char *sName;
	if (!pFile->Read( sName ))
		return false;

	pData = pResourceManager->GetFontResource( sName );
	delete[] sName;
	return pData != NULL;
}

bool UTIL_GetValue( const CTextItem *pTextItem, CFontResource *&pValue )
{
	const char *sName;
	if (!pTextItem->GetValue( sName ))
		return false;

	pValue = pResourceManager->GetFontResource( sName );
	return pValue != NULL;
}