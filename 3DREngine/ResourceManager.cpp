#include "ResourceManager.h"
#include "FileManager.h"
#include "RenderManager.h"

CResourceManager::CResourceManager()
{

}

CAnimationResource *CResourceManager::GetAnimationResource( const char *sName )
{
	for (unsigned int i = 0; i < m_pAnimationResources.size(); i++)
	{
		CAnimationResource *pAnimationResource = m_pAnimationResources[i];
		if (UTIL_streq( pAnimationResource->GetName(), sName ))
			return pAnimationResource;
	}

	if (!pFileManager->Open( sName, EFileType::t_animation ))
		return NULL;

	SAnimationResourceData dData;
	if (!pFileManager->Read( dData ))
		return NULL;

	CAnimationResource *pAnimationResource = new CAnimationResource( dData, sName );

	m_pAnimationResources.push_back( pAnimationResource );
	return pAnimationResource;
}

CCubemapResource *CResourceManager::GetCubemapResource( const char *sName )
{
	for (unsigned int i = 0; i < m_pCubemapResources.size(); i++)
	{
		CCubemapResource *pCubemapResource = m_pCubemapResources[i];
		if (UTIL_streq( pCubemapResource->GetName(), sName ))
			return pCubemapResource;
	}

	if (!pFileManager->Open( sName, EFileType::t_cubemap ))
		return NULL;

	SCubemapResourceData dData;
	if (!pFileManager->Read( dData ))
		return NULL;

	CCubemapResource *pCubemapResource = new CCubemapResource( dData, sName );

	m_pCubemapResources.push_back( pCubemapResource );
	return pCubemapResource;
}


CFontResource *CResourceManager::GetFontResource( const char *sName )
{
	for (unsigned int i = 0; i < m_pFontResources.size(); i++)
	{
		CFontResource *pFontResource = m_pFontResources[i];
		if (UTIL_streq( pFontResource->GetName(), sName ))
			return pFontResource;
	}

	if (!pFileManager->Open( sName, EFileType::t_font ))
		return NULL;

	SFontResourceData dData;
	if (!pFileManager->Read( dData ))
		return NULL;

	CFontResource *pFontResource = new CFontResource( dData, sName );

	m_pFontResources.push_back( pFontResource );
	return pFontResource;
}

CGeometryResource *CResourceManager::GetGeometryResource( const char *sName )
{
	for (unsigned int i = 0; i < m_pGeometryResources.size(); i++)
	{
		CGeometryResource *pGeometryResource = m_pGeometryResources[i];
		if (UTIL_streq( pGeometryResource->GetName(), sName ))
			return pGeometryResource;
	}

	if (!pFileManager->Open( sName, EFileType::t_geometry ))
		return NULL;

	SGeometryResourceData dData;
	if (!pFileManager->Read( dData ))
		return NULL;

	CGeometryResource *pGeometryResource = new CGeometryResource( dData, sName );

	m_pGeometryResources.push_back( pGeometryResource );
	return pGeometryResource;
}

CSkeletonResource *CResourceManager::GetSkeletonResource( const char *sName )
{
	for (unsigned int i = 0; i < m_pSkeletonResources.size(); i++)
	{
		CSkeletonResource *pSkeletonResource = m_pSkeletonResources[i];
		if (UTIL_streq( pSkeletonResource->GetName(), sName ))
			return pSkeletonResource;
	}

	if (!pFileManager->Open( sName, EFileType::t_skeleton ))
		return NULL;

	SSkeletonResourceData dData;
	if (!pFileManager->Read( dData ))
		return NULL;

	CSkeletonResource *pSkeletonResource = new CSkeletonResource( dData, sName );

	m_pSkeletonResources.push_back( pSkeletonResource );
	return pSkeletonResource;
}

CTextureResource *CResourceManager::GetTextureResource( const char *sName )
{
	for (unsigned int i = 0; i < m_pTextureResources.size(); i++)
	{
		CTextureResource *pTextureResource = m_pTextureResources[i];
		if (UTIL_streq( pTextureResource->GetName(), sName ))
			return pTextureResource;
	}

	if (!pFileManager->Open( sName, EFileType::t_texture ))
		return NULL;

	STextureResourceData dData;
	if (!pFileManager->Read( dData ))
		return NULL;

	CTextureResource *pTextureResource = new CTextureResource( dData, sName );

	m_pTextureResources.push_back( pTextureResource );
	return pTextureResource;
}