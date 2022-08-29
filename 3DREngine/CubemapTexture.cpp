#include "CubemapTexture.h"
#include "RenderManager.h"
#include "AssetManager.h"

CCubemapTexture::CCubemapTexture()
{
	m_glID = 0;
}

bool CCubemapTexture::Init( void ) // TODO: potentially optimize
{
	if (!BaseClass::Init())
		return false;

	std::vector<unsigned char *> pData;
	std::vector<GLsizei> glWidth;
	std::vector<GLsizei> glHeight;
	std::vector<unsigned char> ucChannels;

	if (!pAssetManager->LoadImage( GetFilePath(), pData, glWidth, glHeight, ucChannels ))
		return false;

	if (pData.size() != 6)
	{
		for (unsigned int i = 0; i < pData.size(); i++)
			delete[] pData[i];

		return false;
	}

	GLenum tInternalFormat[6];
	GLenum tFormat[6];
	for (unsigned char i = 0; i < 6; i++)
	{
		switch (ucChannels[i])
		{
		case 1:
			tInternalFormat[i] = GL_R8;
			tFormat[i] = GL_RED;
			break;
		case 2:
			tInternalFormat[i] = GL_RG8;
			tFormat[i] = GL_RG;
			break;
		case 3:
			tInternalFormat[i] = GL_RGB8;
			tFormat[i] = GL_RGB;
			break;
		case 4:
			tInternalFormat[i] = GL_RGBA8;
			tFormat[i] = GL_RGBA;
			break;
		default:
			for (unsigned char j = 0; j < 6; j++)
				delete[] pData[j];

			return false;
		}
	}

	glGenTextures( 1, &m_glID );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_glID );
	for (unsigned char i = 0; i < 6; i++)
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, tInternalFormat[i], glWidth[i], glHeight[i], 0, tFormat[i], GL_UNSIGNED_BYTE, pData[i] );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GetFiltered() ? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GetFiltered() ? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	for (unsigned int i = 0; i < 6; i++)
		delete[] pData[i];

	return true;
}

bool CCubemapTexture::Exit( void )
{
	if (m_glID != 0)
	{
		pRenderManager->UnbindTexture( m_glID );

		glDeleteTextures( 1, &m_glID );
	}

	return BaseClass::Exit();
}

int CCubemapTexture::Bind( void )
{
	return pRenderManager->BindTexture( m_glID, GL_TEXTURE_CUBE_MAP );
}