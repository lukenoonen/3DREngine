#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Global.h"
#include "AnimationResource.h"
#include "CubemapResource.h"
#include "GeometryResource.h"
#include "SkeletonResource.h"
#include "TextureResource.h"

enum class ETextureWrap : EBaseEnum
{
	t_repeat = 0,
	t_mirrored,
	t_edge,
	t_border,

	i_count,
	i_invalid = i_count,
};

class CResourceManager
{
public:
	DECLARE_CLASS_NOBASE( CResourceManager )

	CResourceManager();

	CAnimationResource *GetAnimationResource( const char *sName );
	CCubemapResource *GetCubemapResource( const char *sName );
	CGeometryResource *GetGeometryResource( const char *sName );
	CSkeletonResource *GetSkeletonResource( const char *sName );
	CTextureResource *GetTextureResource( const char *sName );

private:
	CAnimationResource *CreateAnimationResource( const char *sName );
	CCubemapResource *CreateCubemapResource( const char *sName );
	CGeometryResource *CreateGeometryResource( const char *sName );
	CSkeletonResource *CreateSkeletonResource( const char *sName );
	CTextureResource *CreateTextureResource( const char *sName );

	bool LoadAnimationResource( float &flTime, std::vector<SAnimationChannel *> &pAnimationChannels );
	bool LoadCubemapResource( unsigned char **pData, GLsizei *glWidth, GLsizei *glHeight, GLenum *tInternalFormat, GLenum *tFormat, bool &bFiltered );
	bool LoadGeometryResource( std::vector<SVertex> &verVertices, std::vector<unsigned int> &uiIndices );
	bool LoadSkeletonResource( SSkeletonNode *&pSkeletonNode, unsigned int &uiSkeletonBonesCount );
	bool LoadTextureResource( unsigned char *&pData, GLsizei &glWidth, GLsizei &glHeight, GLenum &tInternalFormat, GLenum &tFormat, bool &bFiltered, GLint &tTextureWrap, glm::vec4 &vec4BorderColor );

public:
	bool ProcessModel();

private:
	std::vector<CAnimationResource *> m_pAnimationResources;
	std::vector<CCubemapResource *> m_pCubemapResources;
	std::vector<CGeometryResource *> m_pGeometryResources;
	std::vector<CSkeletonResource *> m_pSkeletonResources;
	std::vector<CTextureResource *> m_pTextureResources;
};

/*class CResourceManager // TODO: maybe implement some caching so that files aren't loaded more than once in most cases, if this then change image to texture
{
public:
	DECLARE_CLASS_NOBASE( CResourceManager )

	CResourceManager();

#ifdef PROJECT_ENGINE
	// TODO: check these for any possible memory leaks and also clean them up
	bool LoadAnimation( const char *sFileName, float &flTime, std::vector<SAnimationChannel *> &pAnimationChannels );
	bool LoadGeometry( const char *sFileName, std::vector<SVertex> &verVertices, std::vector<unsigned int> &uiIndices );
	bool LoadImage( const char *sFileName, std::vector<unsigned char *> &pData, std::vector<unsigned int> &uiWidth, std::vector<unsigned int> &uiHeight, std::vector<unsigned int> &uiChannels );
	bool LoadSkeleton( const char *sFileName, SSkeletonNode *&pSkeletonNode, unsigned int &uiSkeletonBonesCount );
#elif PROJECT_RESOURCE

#endif // PROJECT_ENGINE

private:
#ifdef PROJECT_ENGINE
	bool LoadAnimationInternal( float &flTime, std::vector<SAnimationChannel *> &pAnimationChannels );
	bool LoadGeometryInternal( std::vector<SVertex> &verVertices, std::vector<unsigned int> &uiIndices );
	bool LoadImageInternal( std::vector<unsigned char *> &pData, std::vector<unsigned int> &uiWidth, std::vector<unsigned int> &uiHeight, std::vector<unsigned int> &uiChannels );
	bool LoadSkeletonInternal( SSkeletonNode *&pSkeletonNode, unsigned int &uiSkeletonBonesCount );
#elif PROJECT_RESOURCE

#endif // PROJECT_ENGINE
};*/

#endif // RESOURCEMANAGER_H