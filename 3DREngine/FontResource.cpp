#include "FontResource.h"
#include "RenderManager.h"
#include "ResourceManager.h"

CFontChar::CFontChar( const SChar &sChar, CFontResource *pFontResource )
{
	glm::vec2 vec2Min = (glm::vec2)sChar.vec2Offset;
	glm::vec2 vec2Max = vec2Min + (glm::vec2)sChar.vec2Size;

	vec2Min *= g_vec2FlipVertical;
	vec2Max *= g_vec2FlipVertical;

	glm::vec2 vec2BitmapSize = (glm::vec2)pFontResource->GetBitmapSize();
	glm::vec2 vec2TexMin = (glm::vec2)sChar.vec2Position / vec2BitmapSize;
	glm::vec2 vec2TexMax = vec2TexMin + (glm::vec2)sChar.vec2Size / vec2BitmapSize;

	m_verVertices[0] = { glm::vec2( vec2Min.x, vec2Min.y ), glm::vec2( vec2TexMin.x, vec2TexMin.y ) };
	m_verVertices[1] = { glm::vec2( vec2Min.x, vec2Max.y ), glm::vec2( vec2TexMin.x, vec2TexMax.y ) };
	m_verVertices[2] = { glm::vec2( vec2Max.x, vec2Max.y ), glm::vec2( vec2TexMax.x, vec2TexMax.y ) };
	m_verVertices[3] = { glm::vec2( vec2Max.x, vec2Max.y ), glm::vec2( vec2TexMax.x, vec2TexMax.y ) };
	m_verVertices[4] = { glm::vec2( vec2Max.x, vec2Min.y ), glm::vec2( vec2TexMax.x, vec2TexMin.y ) };
	m_verVertices[5] = { glm::vec2( vec2Min.x, vec2Min.y ), glm::vec2( vec2TexMin.x, vec2TexMin.y ) };

	glGenVertexArrays( 1, &m_glVAO );
	glGenBuffers( 1, &m_glVBO );

	glBindVertexArray( m_glVAO );
	glBindBuffer( GL_ARRAY_BUFFER, m_glVBO );
	glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr)(6 * sizeof( SVertex2D )), m_verVertices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( SVertex2D ), (void *)0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( SVertex2D ), (void *)offsetof( SVertex2D, vec2TexCoords ) );
}

CFontChar::~CFontChar()
{
	glDeleteVertexArrays( 1, &m_glVAO );
	glDeleteBuffers( 1, &m_glVBO );
}

void CFontChar::Draw( void ) const
{
	glBindVertexArray( m_glVAO );
	glDrawArrays( GL_TRIANGLES, 0, (GLsizei)6 );
}

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

	for (unsigned int i = 0; i < FONT_CHAR_SIZE; i++)
		m_pFontChars[i] = new CFontChar( m_sChars[i], this );
}

CFontResource::~CFontResource()
{
	pRenderManager->UnbindTexture( m_glBitmapID );
	glDeleteTextures( 1, &m_glBitmapID );

	for (unsigned int i = 0; i < FONT_CHAR_SIZE; i++)
		delete m_pFontChars[i];
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

void CFontResource::DrawChar( char cChar )
{
	m_pFontChars[cChar - FONT_CHAR_START]->Draw();
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