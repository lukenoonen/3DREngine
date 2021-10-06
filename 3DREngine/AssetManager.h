#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Global.h"
#include "Animation.h"
#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Skeleton.h"

class CAssetManager
{
public:
	DECLARE_CLASS_NOBASE( CAssetManager );

	CAssetManager();
	
	CAnimation *GetAnimation( const char *sPath );
	CGeometry *GetGeometry( const char *sPath );
	CMaterial *GetMaterial( const char *sPath );
	CMesh *GetMesh( const char *sPath );
	CModel *GetModel( const char *sPath );
	CTexture *GetTexture( const char *sPath );
	CSkeleton *GetSkeleton( const char *sPath );

	void CheckAnimation( CAnimation *pAnimation );
	void CheckGeometry( CGeometry *pGeometry );
	void CheckMaterial( CMaterial *pMaterial );
	void CheckMesh( CMesh *pMesh );
	void CheckModel( CModel *pModel );
	void CheckTexture( CTexture *pTexture );
	void CheckSkeleton( CSkeleton *pSkeleton );

private:
	CAnimation *CreateAnimation( const char *sPath );
	CGeometry *CreateGeometry( const char *sPath );
	CMaterial *CreateMaterial( const char *sPath );
	CMesh *CreateMesh( const char *sPath );
	CModel *CreateModel( const char *sPath );
	CTexture *CreateTexture( const char *sPath );
	CSkeleton *CreateSkeleton( const char *sPath );

private:
	std::vector<CAnimation *> m_pAnimations;
	std::vector<CGeometry *> m_pGeometries;
	std::vector<CMaterial *> m_pMaterials;
	std::vector<CMesh *> m_pMeshes;
	std::vector<CModel *> m_pModels;
	std::vector<CTexture *> m_pTextures;
	std::vector<CSkeleton *> m_pSkeletons;

public:
	int BindTexture( unsigned int uiTextureID, unsigned int uiTextureType );
	void UnbindTexture( unsigned int uiTextureID );
	void UnbindAllTextures( void );

private:
	std::unordered_map<unsigned int, int> m_mapTextureIDToIndex;
	std::unordered_map<int, unsigned int> m_mapIndexToTextureID;
	unsigned int m_uiTextureIndex;
	unsigned int m_uiMaxTextures;
};

#endif // ASSETMANAGER_H