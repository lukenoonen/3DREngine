#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Global.h"
#include "Animation.h"
#include "Geometry.h"
#include "Material.h"
#include "Texture.h"
#include "Skeleton.h"

class CAssetManager
{
public:
	DECLARE_CLASS_NOBASE( CAssetManager );

	CAssetManager();

	CBaseAsset *GetAsset( const char *sPath );

	void CheckAsset( CBaseAsset *pAsset );

	int BindTexture( unsigned int uiTextureID, unsigned int uiTextureType );
	void UnbindTexture( unsigned int uiTextureID );
	void UnbindAllTextures( void );

private:
	CAnimation *CreateAnimation( const char *sPath );
	CGeometry *CreateGeometry( const char *sPath );
	// CMaterial *CreateMaterial( const char *sPath ); // TODO: replace this with an entity, remove all traces of materials as they were
	CTexture *CreateTexture( const char *sPath );
	CSkeleton *CreateSkeleton( const char *sPath );

private:
	std::vector<CBaseAsset *> m_pAssets[(EBaseEnum)EAssetType::i_count];

	std::unordered_map<unsigned int, int> m_mapTextureIDToIndex;
	std::unordered_map<int, unsigned int> m_mapIndexToTextureID;
	unsigned int m_uiTextureIndex;
	unsigned int m_uiMaxTextures;
};

#endif // ASSETMANAGER_H