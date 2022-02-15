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

CBaseAsset *CAssetManager::GetAsset( const char *sPath )
{
	const char *sExtension = UTIL_extn( sPath );
	if (!sExtension)
		return NULL;

	EAssetType eAssetType = UTIL_AssetTypeExtensionToEnum( sExtension );
	if (eAssetType == EAssetType::i_invalid)
		return NULL;

	std::vector<CBaseAsset *> &pAssets = m_pAssets[(EBaseEnum)eAssetType];

	for (unsigned int i = 0; i < (unsigned int)pAssets.size(); i++)
	{
		CBaseAsset *pAsset = pAssets[i];
		if (UTIL_streq( pAsset->GetPath(), sPath ))
		{
			pFileManager->CloseFile();
			return pAsset;
		}
	}

	if (!pFileManager->OpenFile( sPath ))
		return NULL;

	CBaseAsset *pAsset = NULL;

	switch (eAssetType)
	{
	case EAssetType::t_animation:
	{
		pAsset = CreateAnimation( sPath );
		break;
	}
	case EAssetType::t_geometry:
	{
		pAsset = CreateGeometry( sPath );
		break;
	}
	// case EAssetType::t_material:
	// {
	// 	pAsset = CreateMaterial( sPath );
	// 	break;
	// }
	case EAssetType::t_skeleton:
	{
		pAsset = CreateSkeleton( sPath );
		break;
	}
	case EAssetType::t_texture:
	{
		pAsset = CreateTexture( sPath );
		break;
	}
	}

	pFileManager->CloseFile();

	if (!pAsset)
		return NULL;

	pAssets.push_back( pAsset );
	return pAsset;
}

void CAssetManager::CheckAsset( CBaseAsset *pAsset )
{
	if (!pAsset->IsActive())
	{
		std::vector<CBaseAsset *> &pAssets = m_pAssets[(EBaseEnum)pAsset->GetAssetType()];
		for (unsigned int i = 0; i < (unsigned int)pAssets.size(); i++)
		{
			if (pAssets[i] == pAsset)
			{
				delete pAssets[i];
				pAssets.erase( pAssets.begin() + i );
				break;
			}
		}
	}
}

int CAssetManager::BindTexture( unsigned int uiTextureID, unsigned int uiTextureType )
{
	std::unordered_map<unsigned int, int>::const_iterator itTextureIDSearch = m_mapTextureIDToIndex.find( uiTextureID );
	if (itTextureIDSearch == m_mapTextureIDToIndex.end())
	{
		unsigned int uiPrevTextureIndex = m_uiTextureIndex;
		m_uiTextureIndex = (m_uiTextureIndex + 1) % m_uiMaxTextures;

		std::unordered_map<int, unsigned int>::const_iterator itIndexSearch = m_mapIndexToTextureID.find( uiPrevTextureIndex );
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
	std::unordered_map<unsigned int, int>::const_iterator itTextureIDSearch = m_mapTextureIDToIndex.find( uiTextureID );
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

CAnimation *CAssetManager::CreateAnimation( const char *sPath )
{
	float flTime;
	std::vector<SAnimationChannel *> pAnimationChannels;

	if (!pFileManager->Read( flTime ))
		return NULL;

	unsigned int uiAnimationChannelSlots;
	if (!pFileManager->Read( uiAnimationChannelSlots ))
		return NULL;
	pAnimationChannels.resize( uiAnimationChannelSlots, NULL );

	unsigned int uiAnimationChannelCount;
	if (!pFileManager->Read( uiAnimationChannelCount ))
		return NULL;

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

			return NULL;
		}

		pAnimationChannels[uiAnimationChannelSlot] = new SAnimationChannel;
		SAnimationChannel *pAnimationChannel = pAnimationChannels[uiAnimationChannelSlot];

		unsigned int uiPositionsCount;
		if (!pFileManager->Read( uiPositionsCount ))
		{
			for (unsigned int j = 0; j < uiAnimationChannelSlots; j++)
			{
				if (pAnimationChannels[j])
					delete pAnimationChannels[j];
			}

			return NULL;
		}

		pAnimationChannel->flPositionTimes.resize( uiPositionsCount );
		pAnimationChannel->vec3Positions.resize( uiPositionsCount );

		for (unsigned int j = 0; j < uiPositionsCount; j++)
		{
			if (!pFileManager->Read( pAnimationChannel->flPositionTimes[j] ) || !pFileManager->Read( pAnimationChannel->vec3Positions[j] ))
			{
				for (unsigned int k = 0; k < uiAnimationChannelSlots; k++)
				{
					if (pAnimationChannels[k])
						delete pAnimationChannels[k];
				}

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

			return NULL;
		}

		pAnimationChannel->flRotationTimes.resize( uiRotationsCount );
		pAnimationChannel->qRotations.resize( uiRotationsCount );

		for (unsigned int j = 0; j < uiRotationsCount; j++)
		{
			if (!pFileManager->Read( pAnimationChannel->flRotationTimes[j] ) || !pFileManager->Read( pAnimationChannel->qRotations[j].w ) || !pFileManager->Read( pAnimationChannel->qRotations[j].x ) || !pFileManager->Read( pAnimationChannel->qRotations[j].y ) || !pFileManager->Read( pAnimationChannel->qRotations[j].z ))
			{
				for (unsigned int k = 0; k < uiAnimationChannelSlots; k++)
				{
					if (pAnimationChannels[k])
						delete pAnimationChannels[k];
				}

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

			return NULL;
		}

		pAnimationChannel->flScaleTimes.resize( uiScalesCount );
		pAnimationChannel->vec3Scales.resize( uiScalesCount );

		for (unsigned int j = 0; j < uiScalesCount; j++)
		{
			if (!pFileManager->Read( pAnimationChannel->flScaleTimes[j] ) || !pFileManager->Read( pAnimationChannel->vec3Scales[j] ))
			{
				for (unsigned int k = 0; k < uiAnimationChannelSlots; k++)
				{
					if (pAnimationChannels[k])
						delete pAnimationChannels[k];
				}

				return NULL;
			}
		}
	}

	return new CAnimation( flTime, pAnimationChannels, sPath );
}

CGeometry *CAssetManager::CreateGeometry( const char *sPath )
{
	std::vector<SVertex> verVertices;
	std::vector<unsigned int> uiIndices;

	unsigned int uiVertexCount;
	if (!pFileManager->Read( uiVertexCount ))
		return NULL;

	verVertices.resize( uiVertexCount );

	for (unsigned int i = 0; i < uiVertexCount; i++)
	{
		SVertex &verVertex = verVertices[i];
		unsigned int uiNumAffectingBones;
		if (!pFileManager->Read( verVertex.vec3Position ) || !pFileManager->Read( verVertex.vec3Normal ) || !pFileManager->Read( verVertex.vec3Tangent ) || !pFileManager->Read( verVertex.vec3Bitangent ) || !pFileManager->Read( verVertex.vec2TexCoords ) || !pFileManager->Read( uiNumAffectingBones ))
			return NULL;

		for (unsigned int j = 0; j < uiNumAffectingBones; j++)
		{
			if (!pFileManager->Read( verVertex.vec4BoneIDs[j] ) || !pFileManager->Read( verVertex.vec4Weights[j] ))
				return NULL;
		}
	}

	unsigned int uiFacesCount;
	if (!pFileManager->Read( uiFacesCount ))
		return NULL;

	for (unsigned int i = 0; i < uiFacesCount; i++)
	{
		unsigned int uiIndicesCount;
		if (!pFileManager->Read( uiIndicesCount ))
			return NULL;

		for (unsigned int j = 0; j < uiIndicesCount; j++)
		{
			unsigned int uiIndex;
			if (!pFileManager->Read( uiIndex ))
				return NULL;

			uiIndices.push_back( uiIndex );
		}
	}

	return new CGeometry( verVertices, uiIndices, sPath );
}

/*CMaterial *CAssetManager::CreateMaterial(const char *sPath)
{
	if (!pFileManager->OpenFile( sPath ))
		return NULL;

	EMaterialType eMaterialType;
	if (!pFileManager->Read( eMaterialType ))
	{
		pFileManager->CloseFile();
		return NULL;
	}

	CMaterial *pNewMaterial = NULL;
	switch (eMaterialType)
	{
	case EMaterialType::t_lit:
	{
		CTexture *pDiffuse;
		CTexture *pSpecular = NULL;
		CTexture *pNormal = NULL;
		float flShininess;
		CTexture *pCamera = NULL;
		glm::vec2 vec2TextureScale;
		bool bRecieveShadows;
		bool bCastShadows;

		char *sTexturePath;

		if (!pFileManager->Read( sTexturePath ))
		{
			pFileManager->CloseFile();
			return NULL;
		}

		pDiffuse = GetTexture( sTexturePath );
		delete[] sTexturePath;

		if (!pDiffuse)
		{
			pFileManager->CloseFile();
			return NULL;
		}

		if (!pFileManager->Read( sTexturePath ) || !pFileManager->Read( flShininess ))
		{
			CheckTexture( pDiffuse );
			pFileManager->CloseFile();
			return NULL;
		}

		if (*sTexturePath)
		{
			pSpecular = GetTexture( sTexturePath );
			delete[] sTexturePath;

			if (!pSpecular)
			{
				CheckTexture( pDiffuse );
				pFileManager->CloseFile();
				return NULL;
			}
		}
		else
		{
			delete[] sTexturePath;
		}

		if (!pFileManager->Read( sTexturePath ))
		{
			CheckTexture( pDiffuse );
			if (pSpecular) CheckTexture( pSpecular );
			pFileManager->CloseFile();
			return NULL;
		}

		if (*sTexturePath)
		{
			pNormal = GetTexture( sTexturePath );
			delete[] sTexturePath;

			if (!pNormal)
			{
				CheckTexture( pDiffuse );
				if (pSpecular) CheckTexture( pSpecular );
				pFileManager->CloseFile();
				return NULL;
			}
		}
		else
		{
			delete[] sTexturePath;
		}

		if (!pFileManager->Read( sTexturePath ))
		{
			CheckTexture( pDiffuse );
			if (pSpecular) CheckTexture( pSpecular );
			if (pNormal) CheckTexture( pNormal );
			pFileManager->CloseFile();
			return NULL;
		}

		if (*sTexturePath)
		{
			pCamera = GetTexture( sTexturePath );
			delete[] sTexturePath;

			if (!pCamera)
			{
				CheckTexture( pDiffuse );
				if (pSpecular) CheckTexture( pSpecular );
				if (pNormal) CheckTexture( pNormal );
				pFileManager->CloseFile();
				return NULL;
			}
		}
		else
		{
			delete[] sTexturePath;
		}

		if (!pFileManager->Read( vec2TextureScale ) || !pFileManager->Read( bCastShadows ) || !pFileManager->Read( bRecieveShadows ))
		{
			CheckTexture( pDiffuse );
			if (pSpecular) CheckTexture( pSpecular );
			if (pNormal) CheckTexture( pNormal );
			if (pCamera) CheckTexture( pCamera );
			pFileManager->CloseFile();
			return NULL;
		}

		pNewMaterial = new CLitMaterial( pDiffuse, pSpecular, flShininess, pNormal, pCamera, vec2TextureScale, bCastShadows, bRecieveShadows, sPath );
		break;
	}
	case EMaterialType::t_unlit:
	{
		CTexture *pDiffuse;
		glm::vec2 vec2TextureScale;

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

		if (!pFileManager->Read( vec2TextureScale ))
		{
			CheckTexture( pDiffuse );
			pFileManager->CloseFile();
			return NULL;
		}

		pNewMaterial = new CUnlitMaterial( pDiffuse, vec2TextureScale, sPath );
		break;
	}
	case EMaterialType::t_skybox:
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
}*/

CTexture *CAssetManager::CreateTexture( const char *sPath )
{
	ETextureType eTextureType;
	bool bFilter;
	GLint tFilter;

	if (!pFileManager->Read( eTextureType ) || !pFileManager->Read( bFilter ))
		return NULL;

	tFilter = bFilter ? GL_LINEAR : GL_NEAREST;

	unsigned int uiTextureID;

	switch (eTextureType)
	{
	case ETextureType::t_2d:
	{
		ETextureWrap eTextureWrap;
		glm::vec4 vec4BorderColor;

		char *sImagePath;
		unsigned int uiWidth;
		unsigned int uiHeight;
		unsigned int uiChannels;
		unsigned char *pData;

		if (!pFileManager->Read( eTextureWrap ) || (eTextureWrap == ETextureWrap::t_border && !pFileManager->Read( vec4BorderColor )) || !pFileManager->Read( sImagePath ))
			return NULL;

		if (!pFileManager->OpenFile( sImagePath ))
		{
			delete[] sImagePath;
			return NULL;
		}

		delete[] sImagePath;

		if (!pFileManager->Read( uiWidth ) || !pFileManager->Read( uiHeight ) || !pFileManager->Read( uiChannels ) || !pFileManager->Read( pData, uiWidth * uiHeight * uiChannels ))
		{
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
			return NULL;
		}

		GLint tTextureWrapType;
		switch (eTextureWrap)
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
			delete[] pData;
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
		if (eTextureWrap == ETextureWrap::t_border)
			glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr( vec4BorderColor ) );

		delete[] pData;
		break;
	}
	case ETextureType::t_cubemap:
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

				return NULL;
			}

			if (!pFileManager->OpenFile( sImagePath ))
			{
				delete[] sImagePath;
				for (unsigned int j = 0; j < i; j++)
					delete[] pData[j];

				return NULL;
			}

			delete[] sImagePath;

			if (!pFileManager->Read( uiWidth[i] ) || !pFileManager->Read( uiHeight[i] ) || !pFileManager->Read( uiChannels[i] ) || !pFileManager->Read( pData[i], uiWidth[i] * uiHeight[i] * uiChannels[i] ))
			{
				for (unsigned int j = 0; j < i; j++)
					delete[] pData[j];

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
	case ETextureType::i_invalid:
	{
		return NULL;
	}
	}

	return new CTexture( uiTextureID, sPath );
}

CSkeleton *CAssetManager::CreateSkeleton( const char *sPath )
{
	std::vector<SSkeletonBone *> pSkeletonBones;
	std::vector<SSkeletonNode *> pSkeletonNodes;

	unsigned int uiSkeletonBonesCount;
	if (!pFileManager->Read( uiSkeletonBonesCount ))
		return NULL;

	pSkeletonBones.resize( uiSkeletonBonesCount );

	for (unsigned int i = 0; i < uiSkeletonBonesCount; i++)
	{
		pSkeletonBones[i] = new SSkeletonBone;
		SSkeletonBone *pSkeletonBone = pSkeletonBones[i];

		pSkeletonBone->uiIndex = i;

		if (!pFileManager->Read( pSkeletonBone->matOffset ))
		{
			for (unsigned int j = 0; j <= i; j++)
				delete pSkeletonBones[j];

			return NULL;
		}
	}

	unsigned int uiSkeletonNodesCount;
	if (!pFileManager->Read( uiSkeletonNodesCount ))
	{
		for (unsigned int i = 0; i < uiSkeletonBonesCount; i++)
			delete pSkeletonBones[i];

		return NULL;
	}

	pSkeletonNodes.resize( uiSkeletonNodesCount );

	for (unsigned int i = 0; i < uiSkeletonNodesCount; i++)
	{
		pSkeletonNodes[i] = new SSkeletonNode;
		SSkeletonNode *pSkeletonNode = pSkeletonNodes[i];

		pSkeletonNode->uiIndex = i;
		pSkeletonNode->pSkeletonBone = NULL;

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

				return NULL;
			}

			pSkeletonNode->pSkeletonBone = pSkeletonBones[uiSkeletonBoneIndex];
			if (!pSkeletonNode->pSkeletonBone)
			{
				for (unsigned int j = 0; j < uiSkeletonBonesCount; j++)
				{
					if (pSkeletonBones[j])
						delete pSkeletonBones[j];
				}

				for (unsigned int j = 0; j < i; j++)
					delete pSkeletonNodes[j];

				return NULL;
			}
			else
			{
				pSkeletonBones[uiSkeletonBoneIndex] = NULL;
			}
		}
	}

	for (unsigned int i = 0; i < uiSkeletonBonesCount; i++)
	{
		if (pSkeletonBones[i])
			delete pSkeletonBones[i];
	}

	std::vector<std::vector<SSkeletonNode *>> pChildren;
	pChildren.resize( uiSkeletonNodesCount );

	for (unsigned int i = 0; i < uiSkeletonNodesCount; i++)
	{
		unsigned int uiChildrenCount;
		if (!pFileManager->Read( uiChildrenCount ))
		{
			for (unsigned int j = 0; j < uiSkeletonNodesCount; j++)
				delete pSkeletonNodes[j];

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

				return NULL;
			}

			pChildren[i][j] = pSkeletonNodes[uiChildIndex];
		}
	}

	for (unsigned int i = 0; i < uiSkeletonNodesCount; i++)
		pSkeletonNodes[i]->pChildren = pChildren[i];

	return new CSkeleton( pSkeletonNodes[0], uiSkeletonBonesCount, sPath );
}