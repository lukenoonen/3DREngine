#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Global.h"
#include "Texture.h"

class CTextureManager
{
public:
	DECLARE_CLASS_NOBASE( CTextureManager );

	CTextureManager();
	~CTextureManager();

	void Reset( void );

	CTexture *GetTexture( const char *sPath );
	CTexture *GetCubemap( const char *sPaths[] );

	int BindTexture( unsigned int uiTextureID, unsigned int uiTextureType );
	void UnbindTexture( unsigned int uiTextureID );
	void UnbindAllTextures( void );

	int GetMaxTextures( void ) const;
	int GetGenerateTexture( void ) const;

private:
	CTexture *CreateTexture( const char *sPath );
	CTexture *CreateCubemap( const char **sPaths );

private:
	std::vector<CTexture *> m_pTextures;

	std::unordered_map<unsigned int, int> m_mapTextureIDToIndex;
	std::unordered_map<int, unsigned int> m_mapIndexToTextureID;
	int m_iTextureIndex;

	int m_iMaxTextures;
};

#endif // TEXTUREMANAGER_H