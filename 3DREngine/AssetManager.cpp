#include "AssetManager.h"
#include "FileManager.h"
#include "SharedAsset.h"
#include "LitMaterial.h"
#include "UnlitMaterial.h"
#include "SkyboxMaterial.h"

CAssetManager::CAssetManager()
{
	GLint iTempMaxTextures;
	glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &iTempMaxTextures );
	m_uiMaxTextures = (unsigned int)iTempMaxTextures - 1;

	glActiveTexture( GL_TEXTURE0 + m_uiMaxTextures );

	m_uiTextureIndex = 0;
}

CAnimation *CAssetManager::GetAnimation( const char *sPath )
{
	for (unsigned int i = 0; i < (unsigned int)m_pAnimations.size(); i++)
	{
		CAnimation *pAnimation = m_pAnimations[i];
		if (UTIL_streq( pAnimation->GetPath(), sPath ))
			return pAnimation;
	}

	CAnimation *pAnimation = CreateAnimation( sPath );
	if (!pAnimation)
		return NULL;

	m_pAnimations.push_back( pAnimation );
	return pAnimation;
}

CGeometry *CAssetManager::GetGeometry( const char *sPath )
{
	for (unsigned int i = 0; i < (unsigned int)m_pGeometries.size(); i++)
	{
		CGeometry *pGeometry = m_pGeometries[i];
		if (UTIL_streq( pGeometry->GetPath(), sPath ))
			return pGeometry;
	}

	CGeometry *pGeometry = CreateGeometry( sPath );
	if (!pGeometry)
		return NULL;

	m_pGeometries.push_back( pGeometry );
	return pGeometry;
}

CMaterial *CAssetManager::GetMaterial( const char *sPath )
{
	for (unsigned int i = 0; i < (unsigned int)m_pMaterials.size(); i++)
	{
		CMaterial *pMaterial = m_pMaterials[i];
		if (UTIL_streq( pMaterial->GetPath(), sPath ))
			return pMaterial;
	}

	CMaterial *pMaterial = CreateMaterial( sPath );
	if (!pMaterial)
		return NULL;

	m_pMaterials.push_back( pMaterial );
	return pMaterial;
}

CMesh *CAssetManager::GetMesh( const char *sPath )
{
	for (unsigned int i = 0; i < (unsigned int)m_pMeshes.size(); i++)
	{
		CMesh *pMesh = m_pMeshes[i];
		if (UTIL_streq( pMesh->GetPath(), sPath ))
			return pMesh;
	}

	CMesh *pMesh = CreateMesh( sPath );
	if (!pMesh)
		return NULL;

	m_pMeshes.push_back( pMesh );
	return pMesh;
}

CModel *CAssetManager::GetModel( const char *sPath )
{
	for (unsigned int i = 0; i < (unsigned int)m_pModels.size(); i++)
	{
		CModel *pModel = m_pModels[i];
		if (UTIL_streq( pModel->GetPath(), sPath ))
			return pModel;
	}

	CModel *pModel = CreateModel( sPath );
	if (!pModel)
		return NULL;

	m_pModels.push_back( pModel );
	return pModel;
}

CTexture *CAssetManager::GetTexture( const char *sPath )
{
	for (unsigned int i = 0; i < (unsigned int)m_pTextures.size(); i++)
	{
		CTexture *pTexture = m_pTextures[i];
		if (UTIL_streq( pTexture->GetPath(), sPath ))
			return pTexture;
	}

	CTexture *pTexture = CreateTexture( sPath );
	if (!pTexture)
		return NULL;

	m_pTextures.push_back( pTexture );
	return pTexture;
}

CSkeleton *CAssetManager::GetSkeleton( const char *sPath )
{
	for (unsigned int i = 0; i < (unsigned int)m_pSkeletons.size(); i++)
	{
		CSkeleton *pSkeleton = m_pSkeletons[i];
		if (UTIL_streq( pSkeleton->GetPath(), sPath ))
			return pSkeleton;
	}

	CSkeleton *pSkeleton = CreateSkeleton( sPath );
	if (!pSkeleton)
		return NULL;

	m_pSkeletons.push_back( pSkeleton );
	return pSkeleton;
}

void CAssetManager::CheckAnimation( CAnimation *pAnimation )
{
	if (!pAnimation->IsActive())
	{
		for (unsigned int i = 0; i < (unsigned int)m_pAnimations.size(); i++)
		{
			if (m_pAnimations[i] == pAnimation)
			{
				delete m_pAnimations[i];
				m_pAnimations.erase( m_pAnimations.begin() + i );
				break;
			}
		}
	}
}

void CAssetManager::CheckGeometry( CGeometry *pGeometry )
{
	if (!pGeometry->IsActive())
	{
		for (unsigned int i = 0; i < (unsigned int)m_pGeometries.size(); i++)
		{
			if (m_pGeometries[i] == pGeometry)
			{
				delete m_pGeometries[i];
				m_pGeometries.erase( m_pGeometries.begin() + i );
				break;
			}
		}
	}
}

void CAssetManager::CheckMaterial( CMaterial *pMaterial )
{
	if (!pMaterial->IsActive())
	{
		for (unsigned int i = 0; i < (unsigned int)m_pMaterials.size(); i++)
		{
			if (m_pMaterials[i] == pMaterial)
			{
				delete m_pMaterials[i];
				m_pMaterials.erase( m_pMaterials.begin() + i );
				break;
			}
		}
	}
}

void CAssetManager::CheckMesh( CMesh *pMesh )
{
	if (!pMesh->IsActive())
	{
		for (unsigned int i = 0; i < (unsigned int)m_pMeshes.size(); i++)
		{
			if (m_pMeshes[i] == pMesh)
			{
				delete m_pMeshes[i];
				m_pMeshes.erase( m_pMeshes.begin() + i );
				break;
			}
		}
	}
}

void CAssetManager::CheckModel( CModel *pModel )
{
	if (!pModel->IsActive())
	{
		for (unsigned int i = 0; i < (unsigned int)m_pModels.size(); i++)
		{
			if (m_pModels[i] == pModel)
			{
				delete m_pModels[i];
				m_pModels.erase( m_pModels.begin() + i );
				break;
			}
		}
	}
}

void CAssetManager::CheckTexture( CTexture *pTexture )
{
	if (!pTexture->IsActive())
	{
		for (unsigned int i = 0; i < (unsigned int)m_pTextures.size(); i++)
		{
			if (m_pTextures[i] == pTexture)
			{
				delete m_pTextures[i];
				m_pTextures.erase( m_pTextures.begin() + i );
				break;
			}
		}
	}
}

void CAssetManager::CheckSkeleton( CSkeleton *pSkeleton )
{
	if (!pSkeleton->IsActive())
	{
		for (unsigned int i = 0; i < (unsigned int)m_pSkeletons.size(); i++)
		{
			if (m_pSkeletons[i] == pSkeleton)
			{
				delete m_pSkeletons[i];
				m_pSkeletons.erase( m_pSkeletons.begin() + i );
				break;
			}
		}
	}
}

#include <iostream>

CAnimation *CAssetManager::CreateAnimation( const char *sPath )
{
	float flTime;
	std::vector<CAnimationChannel *> pAnimationChannels;

	if (!pFileManager->OpenFile( sPath ))
		return NULL;

	if (!pFileManager->Read( flTime ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	unsigned int uiAnimationChannelSlots;
	if (!pFileManager->Read( uiAnimationChannelSlots ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	pAnimationChannels.resize( uiAnimationChannelSlots, NULL );

	unsigned int uiAnimationChannelCount;
	if (!pFileManager->Read( uiAnimationChannelCount ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	for (unsigned int i = 0; i < uiAnimationChannelCount; i++)
	{
		unsigned int uiAnimationChannelSlot;
		if (!pFileManager->Read( uiAnimationChannelSlot ))
		{
			for (unsigned int j = 0; j < uiAnimationChannelSlots; j++)
			{
				if (pAnimationChannels[j])
					delete pAnimationChannels[j];
			}

			pFileManager->CloseFile();
			return NULL;
		}

		std::vector<float> flPositionTimes;
		std::vector<glm::vec3> vecPositions;
		std::vector<float> flRotationTimes;
		std::vector<glm::quat> qRotations;
		std::vector<float> flScaleTimes;
		std::vector<glm::vec3> vecScales;

		unsigned int uiPositionsCount;
		if (!pFileManager->Read( uiPositionsCount ))
		{
			for (unsigned int j = 0; j < uiAnimationChannelSlots; j++)
			{
				if (pAnimationChannels[j])
					delete pAnimationChannels[j];
			}

			pFileManager->CloseFile();
			return NULL;
		}

		flPositionTimes.resize( uiPositionsCount );
		vecPositions.resize( uiPositionsCount );

		for (unsigned int j = 0; j < uiPositionsCount; j++)
		{
			if (!pFileManager->Read( flPositionTimes[j] ) || !pFileManager->Read( vecPositions[j] ))
			{
				for (unsigned int k = 0; k < uiAnimationChannelSlots; k++)
				{
					if (pAnimationChannels[k])
						delete pAnimationChannels[k];
				}

				pFileManager->CloseFile();
				return NULL;
			}
		}

		unsigned int uiRotationsCount;
		if (!pFileManager->Read( uiRotationsCount ))
		{
			for (unsigned int j = 0; j < uiAnimationChannelSlots; j++)
			{
				if (pAnimationChannels[j])
					delete pAnimationChannels[j];
			}

			pFileManager->CloseFile();
			return NULL;
		}

		flRotationTimes.resize( uiRotationsCount );
		qRotations.resize( uiRotationsCount );

		for (unsigned int j = 0; j < uiRotationsCount; j++)
		{
			if (!pFileManager->Read( flRotationTimes[j] ) || !pFileManager->Read( qRotations[j].w ) || !pFileManager->Read( qRotations[j].x ) || !pFileManager->Read( qRotations[j].y ) || !pFileManager->Read( qRotations[j].z ))
			{
				for (unsigned int k = 0; k < uiAnimationChannelSlots; k++)
				{
					if (pAnimationChannels[k])
						delete pAnimationChannels[k];
				}

				pFileManager->CloseFile();
				return NULL;
			}
		}

		unsigned int uiScalesCount;
		if (!pFileManager->Read( uiScalesCount ))
		{
			for (unsigned int j = 0; j < uiAnimationChannelSlots; j++)
			{
				if (pAnimationChannels[j])
					delete pAnimationChannels[j];
			}

			pFileManager->CloseFile();
			return NULL;
		}

		flScaleTimes.resize( uiScalesCount );
		vecScales.resize( uiScalesCount );

		for (unsigned int j = 0; j < uiScalesCount; j++)
		{
			if (!pFileManager->Read( flScaleTimes[j] ) || !pFileManager->Read( vecScales[j] ))
			{
				for (unsigned int k = 0; k < uiAnimationChannelSlots; k++)
				{
					if (pAnimationChannels[k])
						delete pAnimationChannels[k];
				}

				pFileManager->CloseFile();
				return NULL;
			}
		}

		pAnimationChannels[uiAnimationChannelSlot] = new CAnimationChannel( flPositionTimes, vecPositions, flRotationTimes, qRotations, flScaleTimes, vecScales );
	}

	pFileManager->CloseFile();

	return new CAnimation( flTime, pAnimationChannels, sPath );
}

CGeometry *CAssetManager::CreateGeometry( const char *sPath )
{
	std::vector<SVertex> verVertices;
	std::vector<unsigned int> uiIndices;

	if (!pFileManager->OpenFile( sPath ))
		return NULL;

	unsigned int uiVertexCount;
	if (!pFileManager->Read( uiVertexCount ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	verVertices.resize( uiVertexCount );

	for (unsigned int i = 0; i < uiVertexCount; i++)
	{
		SVertex &verVertex = verVertices[i];
		unsigned int uiNumAffectingBones;
		if (!pFileManager->Read( verVertex.vecPosition ) || !pFileManager->Read( verVertex.vecNormal ) || !pFileManager->Read( verVertex.vecTangent ) || !pFileManager->Read( verVertex.vecBitangent ) || !pFileManager->Read( verVertex.vecTexCoords ) || !pFileManager->Read( uiNumAffectingBones ))
		{
			pFileManager->CloseFile();
			return NULL;
		}

		for (unsigned int j = 0; j < uiNumAffectingBones; j++)
		{
			if (!pFileManager->Read( verVertex.vecBoneIDs[j] ) || !pFileManager->Read( verVertex.vecWeights[j] ))
			{
				pFileManager->CloseFile();
				return NULL;
			}
		}
	}

	unsigned int uiFacesCount;
	if (!pFileManager->Read( uiFacesCount ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	for (unsigned int i = 0; i < uiFacesCount; i++)
	{
		unsigned int uiIndiciesCount;
		if (!pFileManager->Read( uiIndiciesCount ))
		{
			pFileManager->CloseFile();
			return NULL;
		}

		for (unsigned int j = 0; j < uiIndiciesCount; j++)
		{
			unsigned int uiIndex;
			if (!pFileManager->Read( uiIndex ))
			{
				pFileManager->CloseFile();
				return NULL;
			}
			uiIndices.push_back( uiIndex );
		}
	}

	pFileManager->CloseFile();

	return new CGeometry( verVertices, uiIndices, sPath );
}

CMaterial *CAssetManager::CreateMaterial( const char *sPath )
{
	if (!pFileManager->OpenFile( sPath ))
		return NULL;

	MaterialType_t tMaterialType;
	if (!pFileManager->Read( tMaterialType ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	CMaterial *pNewMaterial = NULL;
	switch (tMaterialType)
	{
	case MATERIALTYPE_LIT:
	{
		CTexture *pDiffuse;
		CTexture *pSpecular;
		CTexture *pNormal;
		float flShininess;
		glm::vec2 vecTextureScale;

		char *sDiffusePath;
		if (!pFileManager->Read( sDiffusePath ))
		{
			pFileManager->CloseFile();
			return NULL;
		}

		pDiffuse = GetTexture( sDiffusePath );
		delete[] sDiffusePath;

		if (!pDiffuse)
		{
			pFileManager->CloseFile();
			return NULL;
		}

		char *sSpecularPath;
		if (!pFileManager->Read( sSpecularPath ))
		{
			CheckTexture( pDiffuse );
			pFileManager->CloseFile();
			return NULL;
		}

		pSpecular = GetTexture( sSpecularPath );
		delete[] sSpecularPath;

		char *sNormalPath;
		if (!pFileManager->Read( sNormalPath ))
		{
			CheckTexture( pDiffuse );

			if (pSpecular)
				CheckTexture( pSpecular );

			pFileManager->CloseFile();
			return NULL;
		}

		pNormal = GetTexture( sNormalPath );
		delete[] sNormalPath;

		if (!pFileManager->Read( flShininess ) || !pFileManager->Read( vecTextureScale ))
		{
			CheckTexture( pDiffuse );

			if (pSpecular)
				CheckTexture( pSpecular );

			if (pNormal)
				CheckTexture( pNormal );

			pFileManager->CloseFile();
			return NULL;
		}

		pNewMaterial = new CLitMaterial( pDiffuse, pSpecular, pNormal, flShininess, vecTextureScale, sPath );
		break;
	}
	case MATERIALTYPE_UNLIT:
	{
		CTexture *pDiffuse;
		glm::vec2 vecTextureScale;

		char *sDiffusePath;
		if (!pFileManager->Read( sDiffusePath ))
		{
			pFileManager->CloseFile();
			return NULL;
		}

		pDiffuse = GetTexture( sDiffusePath );
		delete[] sDiffusePath;

		if (!pDiffuse)
		{
			pFileManager->CloseFile();
			return NULL;
		}

		if (!pFileManager->Read( vecTextureScale ))
		{
			CheckTexture( pDiffuse );
			pFileManager->CloseFile();
			return NULL;
		}

		pNewMaterial = new CUnlitMaterial( pDiffuse, vecTextureScale, sPath );
		break;
	}
	case MATERIALTYPE_SKYBOX:
	{
		CTexture *pSkybox;

		char *sSkyboxPath;
		if (!pFileManager->Read( sSkyboxPath ))
		{
			pFileManager->CloseFile();
			return NULL;
		}

		pSkybox = GetTexture( sSkyboxPath );
		delete[] sSkyboxPath;

		if (!pSkybox)
		{
			pFileManager->CloseFile();
			return NULL;
		}

		pNewMaterial = new CSkyboxMaterial( pSkybox, sPath );
		break;
	}
	}

	pFileManager->CloseFile();
	return pNewMaterial;
}

CMesh *CAssetManager::CreateMesh( const char *sPath )
{
	CGeometry *pGeometry;
	CMaterial *pMaterial;

	if (!pFileManager->OpenFile( sPath ))
		return NULL;

	char *sGeometryPath;
	if (!pFileManager->Read( sGeometryPath ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	pGeometry = GetGeometry( sGeometryPath );
	delete[] sGeometryPath;
	if (!pGeometry)
	{
		pFileManager->CloseFile();
		return NULL;
	}

	char *sMaterialPath;
	if (!pFileManager->Read( sMaterialPath ))
	{
		CheckGeometry( pGeometry );
		pFileManager->CloseFile();
		return NULL;
	}

	pMaterial = GetMaterial( sMaterialPath );
	delete[] sMaterialPath;
	if (!pMaterial)
	{
		CheckGeometry( pGeometry );
		pFileManager->CloseFile();
		return NULL;
	}

	pFileManager->CloseFile();

	return new CMesh( pGeometry, pMaterial, sPath );
}

CModel *CAssetManager::CreateModel( const char *sPath )
{
	std::vector<CMesh *> pMeshes;
	std::vector<CAnimation *> pAnimations;
	CSkeleton *pSkeleton = NULL;

	if (!pFileManager->OpenFile( sPath ))
		return NULL;

	unsigned int uiMeshCount;
	if (!pFileManager->Read( uiMeshCount ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	pMeshes.resize( uiMeshCount );

	for (unsigned int i = 0; i < uiMeshCount; i++)
	{
		char *sMeshPath;
		if (!pFileManager->Read( sMeshPath ))
		{
			for (unsigned int j = 0; j < i; j++)
				CheckMesh( pMeshes[j] );

			pFileManager->CloseFile();
			return NULL;
		}

		pMeshes[i] = GetMesh( sMeshPath );
		delete[] sMeshPath;

		if (!pMeshes[i])
		{
			for (unsigned int j = 0; j < i; j++)
				CheckMesh( pMeshes[j] );

			pFileManager->CloseFile();
			return NULL;
		}
	}

	bool bIsAnimated;
	if (!pFileManager->Read( bIsAnimated ))
	{
		for (unsigned int j = 0; j < uiMeshCount; j++)
			CheckMesh( pMeshes[j] );

		pFileManager->CloseFile();
		return NULL;
	}

	if (bIsAnimated)
	{
		unsigned int uiAnimationCount;
		if (!pFileManager->Read( uiAnimationCount ))
		{
			for (unsigned int j = 0; j < uiMeshCount; j++)
				CheckMesh( pMeshes[j] );

			pFileManager->CloseFile();
			return NULL;
		}

		pAnimations.resize( uiAnimationCount );

		for (unsigned int i = 0; i < uiAnimationCount; i++)
		{
			char *sAnimationPath;
			if (!pFileManager->Read( sAnimationPath ))
			{
				for (unsigned int j = 0; j < uiMeshCount; j++)
					CheckMesh( pMeshes[j] );

				for (unsigned int j = 0; j < i; j++)
					CheckAnimation( pAnimations[j] );

				pFileManager->CloseFile();
				return NULL;
			}

			pAnimations[i] = GetAnimation( sAnimationPath );
			delete[] sAnimationPath;

			if (!pAnimations[i])
			{
				for (unsigned int j = 0; j < uiMeshCount; j++)
					CheckMesh( pMeshes[j] );

				for (unsigned int j = 0; j < i; j++)
					CheckAnimation( pAnimations[j] );

				pFileManager->CloseFile();
				return NULL;
			}
		}

		char *sSkeletonPath;
		if (!pFileManager->Read( sSkeletonPath ))
		{
			for (unsigned int i = 0; i < uiMeshCount; i++)
				CheckMesh( pMeshes[i] );

			for (unsigned int i = 0; i < uiAnimationCount; i++)
				CheckAnimation( pAnimations[i] );

			pFileManager->CloseFile();
			return NULL;
		}

		pSkeleton = GetSkeleton( sSkeletonPath );
		delete[] sSkeletonPath;

		if (!pSkeleton)
		{
			for (unsigned int i = 0; i < uiMeshCount; i++)
				CheckMesh( pMeshes[i] );

			for (unsigned int i = 0; i < uiAnimationCount; i++)
				CheckAnimation( pAnimations[i] );

			pFileManager->CloseFile();
			return NULL;
		}
	}

	pFileManager->CloseFile();

	return new CModel( pMeshes, pAnimations, pSkeleton, sPath );
}

CTexture *CAssetManager::CreateTexture( const char *sPath )
{
	if (!pFileManager->OpenFile( sPath ))
		return NULL;

	TextureType_t tTextureType;
	bool bFilter;
	GLint tFilter;

	if (!pFileManager->Read( tTextureType ) || !pFileManager->Read( bFilter ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	tFilter = bFilter ? GL_LINEAR : GL_NEAREST;

	unsigned int uiTextureID;

	switch (tTextureType)
	{
	case TEXTURETYPE_2D:
	{
		TextureWrap_t tTextureWrap;
		glm::vec4 vecBorderColor;

		char *sImagePath;
		unsigned int uiWidth;
		unsigned int uiHeight;
		unsigned int uiChannels;
		unsigned char *pData;

		if (!pFileManager->Read( tTextureWrap ) || (tTextureType == TEXTUREWRAP_BORDER && !pFileManager->Read( vecBorderColor )) || !pFileManager->Read( sImagePath ))
		{
			pFileManager->CloseFile();
			return NULL;
		}

		if (!pFileManager->OpenFile( sImagePath ))
		{
			delete[] sImagePath;
			pFileManager->CloseFile();
			return NULL;
		}

		delete[] sImagePath;

		if (!pFileManager->Read( uiWidth ) || !pFileManager->Read( uiHeight ) || !pFileManager->Read( uiChannels ) || !pFileManager->Read( pData, uiWidth * uiHeight * uiChannels ))
		{
			pFileManager->CloseFile();
			pFileManager->CloseFile();
			return NULL;
		}

		pFileManager->CloseFile();

		GLenum tInternalFormat;
		GLenum tFormat;
		switch (uiChannels)
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
			delete[] pData;
			pFileManager->CloseFile();
			return NULL;
		}

		GLint tTextureWrapType;
		switch (tTextureWrap)
		{
		case TEXTUREWRAP_REPEAT:
			tTextureWrapType = GL_REPEAT;
			break;
		case TEXTUREWRAP_MIRRORED:
			tTextureWrapType = GL_MIRRORED_REPEAT;
			break;
		case TEXTUREWRAP_EDGE:
			tTextureWrapType = GL_CLAMP_TO_EDGE;
			break;
		case TEXTUREWRAP_BORDER:
			tTextureWrapType = GL_CLAMP_TO_BORDER;
			break;
		default:
			delete[] pData;
			pFileManager->CloseFile();
			return NULL;
		}

		glGenTextures( 1, &uiTextureID );
		glBindTexture( GL_TEXTURE_2D, uiTextureID );
		glTexImage2D( GL_TEXTURE_2D, 0, tInternalFormat, uiWidth, uiHeight, 0, tFormat, GL_UNSIGNED_BYTE, pData );
		glGenerateMipmap( GL_TEXTURE_2D );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tTextureWrapType );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tTextureWrapType );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tFilter );
		if (tTextureWrap == TEXTUREWRAP_BORDER)
			glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr( vecBorderColor ) );

		delete[] pData;
		break;
	}
	case TEXTURETYPE_CUBEMAP:
	{
		unsigned int uiWidth[6];
		unsigned int uiHeight[6];
		unsigned int uiChannels[6];
		unsigned char *pData[6];

		for (unsigned int i = 0; i < 6; i++)
		{
			char *sImagePath;
			if (!pFileManager->Read( sImagePath ))
			{
				for (unsigned int j = 0; j < i; j++)
					delete[] pData[j];

				pFileManager->CloseFile();
				return NULL;
			}

			if (!pFileManager->OpenFile( sImagePath ))
			{
				delete[] sImagePath;
				for (unsigned int j = 0; j < i; j++)
					delete[] pData[j];

				pFileManager->CloseFile();
				return NULL;
			}

			delete[] sImagePath;

			if (!pFileManager->Read( uiWidth[i] ) || !pFileManager->Read( uiHeight[i] ) || !pFileManager->Read( uiChannels[i] ) || !pFileManager->Read( pData[i], uiWidth[i] * uiHeight[i] * uiChannels[i] ))
			{
				for (unsigned int j = 0; j < i; j++)
					delete[] pData[j];

				pFileManager->CloseFile();
				pFileManager->CloseFile();
				return NULL;
			}

			pFileManager->CloseFile();
		}

		GLenum tInternalFormat[6];
		GLenum tFormat[6];
		for (unsigned int i = 0; i < 6; i++)
		{
			switch (uiChannels[i])
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
				for (unsigned int i = 0; i < 6; i++)
					delete[] pData[i];

				pFileManager->CloseFile();
				return NULL;
			}
		}

		glGenTextures( 1, &uiTextureID );
		glBindTexture( GL_TEXTURE_CUBE_MAP, uiTextureID );
		for (unsigned int i = 0; i < 6; i++)
			glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, tInternalFormat[i], uiWidth[i], uiHeight[i], 0, tFormat[i], GL_UNSIGNED_BYTE, pData[i] );

		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, tFilter );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, tFilter );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

		for (unsigned int i = 0; i < 6; i++)
			delete[] pData[i];

		break;
	}
	case TEXTURETYPE_INVALID:
	{
		pFileManager->CloseFile();
		return NULL;
	}
	}

	pFileManager->CloseFile();

	return new CTexture( uiTextureID, sPath );
}

CSkeleton *CAssetManager::CreateSkeleton( const char *sPath )
{
	std::vector<CSkeletonBone *> pSkeletonBones;
	std::vector<CSkeletonNode *> pSkeletonNodes;

	if (!pFileManager->OpenFile( sPath ))
		return NULL;

	unsigned int uiSkeletonBonesCount;
	if (!pFileManager->Read( uiSkeletonBonesCount ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	pSkeletonBones.resize( uiSkeletonBonesCount );

	for (unsigned int i = 0; i < uiSkeletonBonesCount; i++)
	{
		glm::mat4 matOffset;
		if (!pFileManager->Read( matOffset ))
		{
			for (unsigned int j = 0; j < i; j++)
				delete pSkeletonBones[j];

			pFileManager->CloseFile();
			return NULL;
		}

		pSkeletonBones[i] = new CSkeletonBone( i, matOffset );
	}

	unsigned int uiSkeletonNodesCount;
	if (!pFileManager->Read( uiSkeletonNodesCount ))
	{
		for (unsigned int i = 0; i < uiSkeletonBonesCount; i++)
			delete pSkeletonBones[i];

		pFileManager->CloseFile();
		return NULL;
	}

	pSkeletonNodes.resize( uiSkeletonNodesCount );

	for (unsigned int i = 0; i < uiSkeletonNodesCount; i++)
	{
		CSkeletonBone *pSkeletonBone = NULL;
		bool bHasSkeletonBone;
		if (!pFileManager->Read( bHasSkeletonBone ))
		{
			for (unsigned int j = 0; j < uiSkeletonBonesCount; j++)
			{
				if (pSkeletonBones[j])
					delete pSkeletonBones[j];
			}

			for (unsigned int j = 0; j < i; j++)
				delete pSkeletonNodes[j];

			pFileManager->CloseFile();
			return NULL;
		}

		if (bHasSkeletonBone)
		{
			unsigned int uiSkeletonBoneIndex;
			if (!pFileManager->Read( uiSkeletonBoneIndex ) || uiSkeletonBoneIndex >= uiSkeletonBonesCount)
			{
				for (unsigned int j = 0; j < uiSkeletonBonesCount; j++)
				{
					if (pSkeletonBones[j])
						delete pSkeletonBones[j];
				}

				for (unsigned int j = 0; j < i; j++)
					delete pSkeletonNodes[j];

				pFileManager->CloseFile();
				return NULL;
			}

			pSkeletonBone = pSkeletonBones[uiSkeletonBoneIndex];
			if (!pSkeletonBone)
			{
				for (unsigned int j = 0; j < uiSkeletonBonesCount; j++)
				{
					if (pSkeletonBones[j])
						delete pSkeletonBones[j];
				}

				for (unsigned int j = 0; j < i; j++)
					delete pSkeletonNodes[j];

				pFileManager->CloseFile();
				return NULL;
			}
			else
			{
				pSkeletonBones[uiSkeletonBoneIndex] = NULL;
			}
		}

		pSkeletonNodes[i] = new CSkeletonNode( i, pSkeletonBone );
	}

	for (unsigned int i = 0; i < uiSkeletonBonesCount; i++)
	{
		if (pSkeletonBones[i])
			delete pSkeletonBones[i];
	}

	std::vector<std::vector<CSkeletonNode *>> pChildren;
	pChildren.resize( uiSkeletonNodesCount );

	for (unsigned int i = 0; i < uiSkeletonNodesCount; i++)
	{
		unsigned int uiChildrenCount;
		if (!pFileManager->Read( uiChildrenCount ))
		{
			for (unsigned int j = 0; j < uiSkeletonNodesCount; j++)
				delete pSkeletonNodes[j];

			pFileManager->CloseFile();
			return NULL;
		}

		pChildren[i].resize( uiChildrenCount );

		for (unsigned int j = 0; j < uiChildrenCount; j++)
		{
			unsigned int uiChildIndex;
			if (!pFileManager->Read( uiChildIndex ) || i == uiChildIndex)
			{
				for (unsigned int k = 0; k < uiSkeletonNodesCount; k++)
					delete pSkeletonNodes[k];

				pFileManager->CloseFile();
				return NULL;
			}

			pChildren[i][j] = pSkeletonNodes[uiChildIndex];
		}
	}

	for (unsigned int i = 0; i < uiSkeletonNodesCount; i++)
		pSkeletonNodes[i]->SetChildren( pChildren[i] );

	pFileManager->CloseFile();

	return new CSkeleton( pSkeletonNodes[0], uiSkeletonBonesCount, sPath );
}

int CAssetManager::BindTexture( unsigned int uiTextureID, unsigned int uiTextureType )
{
	std::unordered_map<unsigned int, int>::iterator itTextureIDSearch = m_mapTextureIDToIndex.find( uiTextureID );
	if (itTextureIDSearch == m_mapTextureIDToIndex.end())
	{
		unsigned int uiPrevTextureIndex = m_uiTextureIndex;
		m_uiTextureIndex = (m_uiTextureIndex + 1) % m_uiMaxTextures;

		std::unordered_map<int, unsigned int>::iterator itIndexSearch = m_mapIndexToTextureID.find( uiPrevTextureIndex );
		if (itIndexSearch != m_mapIndexToTextureID.end())
		{
			m_mapTextureIDToIndex.erase( itIndexSearch->second );
			m_mapIndexToTextureID.erase( itIndexSearch );
		}

		m_mapTextureIDToIndex.emplace( uiTextureID, uiPrevTextureIndex );
		m_mapIndexToTextureID.emplace( uiPrevTextureIndex, uiTextureID );

		glActiveTexture( GL_TEXTURE0 + uiPrevTextureIndex );
		glBindTexture( uiTextureType, uiTextureID );
		glActiveTexture( GL_TEXTURE0 + m_uiMaxTextures );

		return uiPrevTextureIndex;
	}

	return itTextureIDSearch->second;
}

void CAssetManager::UnbindTexture( unsigned int uiTextureID )
{
	std::unordered_map<unsigned int, int>::iterator itTextureIDSearch = m_mapTextureIDToIndex.find( uiTextureID );
	if (itTextureIDSearch != m_mapTextureIDToIndex.end())
	{
		m_mapTextureIDToIndex.erase( itTextureIDSearch );
		m_mapIndexToTextureID.erase( itTextureIDSearch->second );
	}
}

void CAssetManager::UnbindAllTextures( void )
{
	m_mapTextureIDToIndex.clear();
	m_mapIndexToTextureID.clear();
	m_uiTextureIndex = 0;
}