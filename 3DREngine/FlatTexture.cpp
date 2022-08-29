#include "FlatTexture.h"
#include "RenderManager.h"
#include "AssetManager.h"

CFlatTexture::CFlatTexture()
{
	m_glID = 0;

	m_eTextureWrap = ETextureWrap::t_repeat;
	m_vec4BorderColor = g_vec4One;
}

bool CFlatTexture::Init( void ) // TODO: potentially optimize
{
	if (!BaseClass::Init())
		return false;

	std::vector<unsigned char *> pData;
	std::vector<GLsizei> glWidth;
	std::vector<GLsizei> glHeight;
	std::vector<unsigned char> ucChannels;

	if (!pAssetManager->LoadImage( GetFilePath(), pData, glWidth, glHeight, ucChannels ))
		return false;

	if (pData.size() != 1)
	{
		for (unsigned int i = 0; i < pData.size(); i++)
			delete[] pData[i];

		return false;
	}

	GLenum tInternalFormat;
	GLenum tFormat;
	switch (ucChannels[0])
	{
	case 1:
		tInternalFormat = GL_R8;
		tFormat = GL_RED;
		break;
	case 2:
		tInternalFormat = GL_RG8;
		tFormat = GL_RG;
		break;
	case 3:
		tInternalFormat = GL_RGB8;
		tFormat = GL_RGB;
		break;
	case 4:
		tInternalFormat = GL_RGBA8;
		tFormat = GL_RGBA;
		break;
	default:
		delete[] pData[0];
		return false;
	}

	GLint tTextureWrapType;
	switch (m_eTextureWrap)
	{
	case ETextureWrap::t_repeat:
		tTextureWrapType = GL_REPEAT;
		break;
	case ETextureWrap::t_mirrored:
		tTextureWrapType = GL_MIRRORED_REPEAT;
		break;
	case ETextureWrap::t_edge:
		tTextureWrapType = GL_CLAMP_TO_EDGE;
		break;
	case ETextureWrap::t_border:
		tTextureWrapType = GL_CLAMP_TO_BORDER;
		break;
	default:
		delete[] pData[0];
		return false;
	}

	glGenTextures( 1, &m_glID );
	glBindTexture( GL_TEXTURE_2D, m_glID );
	glTexImage2D( GL_TEXTURE_2D, 0, tInternalFormat, glWidth[0], glHeight[0], 0, tFormat, GL_UNSIGNED_BYTE, pData[0] );
	glGenerateMipmap( GL_TEXTURE_2D );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tTextureWrapType );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tTextureWrapType );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetFiltered() ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetFiltered() ? GL_LINEAR : GL_NEAREST );
	if (m_eTextureWrap == ETextureWrap::t_border)
		glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr( m_vec4BorderColor ) );

	delete[] pData[0];

	return true;
}

bool CFlatTexture::Exit( void )
{
	if (m_glID != 0)
	{
		pRenderManager->UnbindTexture( m_glID );

		glDeleteTextures( 1, &m_glID );
	}

	return BaseClass::Exit();
}

int CFlatTexture::Bind( void )
{
	return pRenderManager->BindTexture( m_glID, GL_TEXTURE_2D );
}

void CFlatTexture::SetTextureWrap( ETextureWrap eTextureWrap )
{
	m_eTextureWrap = eTextureWrap;
}

void CFlatTexture::SetBorderColor( const glm::vec4 &vec4BorderColor )
{
	m_vec4BorderColor = vec4BorderColor;
}