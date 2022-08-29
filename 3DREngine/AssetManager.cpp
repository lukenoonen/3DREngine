#include "AssetManager.h"
#include "FileManager.h"
#include "RenderManager.h"

CAssetManager::CAssetManager()
{

}

bool CAssetManager::LoadConfig( const char *sFileName, char *&sCommand )
{
	if (!pFileManager->Open( sFileName, EFileType::t_config ))
		return false;

	if (!LoadConfigInternal( sCommand ))
	{
		pFileManager->Close();
		return false;
	}

	pFileManager->Close();
	return true;
}

bool CAssetManager::LoadShader( const char *sFileName, char *&sSource, std::vector<unsigned int> *uiIndices )
{
	const char *sExtension = UTIL_extn( sFileName ); // TODO: Simplify and optimize this
	switch (UTIL_FileTypeExtensionToEnum( sExtension ))
	{
	case EFileType::t_vertexshader:
	case EFileType::t_geometryshader:
	case EFileType::t_fragmentshader:
	{
		break;
	}
	default:
	{
		return false;
	}
	}

	if (!pFileManager->Open( sFileName ))
		return false;

	char *sPreSource;

	if (!LoadShaderInternal( sPreSource ))
	{
		pFileManager->Close();
		return false;
	}

	const char *sReadChar = sPreSource;

	std::vector<char> cSource;

	while (*sReadChar)
	{
		if (*sReadChar == '#')
		{
			const char *sPreProcessorChar = sReadChar + 1;
			int iPreProcessorSize = (int)(UTIL_strchr( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);
			if (UTIL_streq( "subshader", sPreProcessorChar, UTIL_Min( sizeof( "subshader" ), iPreProcessorSize ) - 1 ))
			{
				sPreProcessorChar = UTIL_strchri( sPreProcessorChar + iPreProcessorSize, " \t\n" );
				int iSubShaderSize = (int)(UTIL_strchr( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);

				for (EBaseEnum i = 0; i < (EBaseEnum)EShaderPreprocessor::i_count; i++)
				{
					if (uiIndices[i].empty() && UTIL_streq( g_sShaderPreprocessorNames[i], sPreProcessorChar, UTIL_Min( sizeof( g_sShaderPreprocessorNames[i] ), iSubShaderSize ) - 1 ))
					{
						uiIndices[i].resize( g_eShaderPreprocessorCount[i] );

						for (EBaseEnum j = 0; j < g_eShaderPreprocessorCount[i]; j++)
						{
							const char *sSubShaderDefines = g_pShaderPreprocessorDefines[i][j];
							while (*sSubShaderDefines) cSource.push_back( *sSubShaderDefines++ );
							uiIndices[i][j] = (unsigned short)cSource.size() - 2;
						}
					}
				}

				sReadChar = sPreProcessorChar + iSubShaderSize + 1;
				continue;
			}
		}

		cSource.push_back( *sReadChar++ );
	}

	delete[] sPreSource;

	unsigned short usSize = (unsigned short)cSource.size();
	sSource = new char[usSize + 1];
	for (unsigned short i = 0; i < usSize; i++)
		sSource[i] = cSource[i];

	sSource[usSize] = '\0';

	pFileManager->Close();
	return true;
}

bool CAssetManager::LoadAnimation( const char *sFileName, float &flTime, std::vector<SAnimationChannel *> &pAnimationChannels )
{
	if (!pFileManager->Open( sFileName, EFileType::t_animation ))
		return false;

	if (!LoadAnimationInternal( flTime, pAnimationChannels ))
	{
		for (unsigned short i = 0; i < (unsigned short)pAnimationChannels.size(); i++)
		{
			if (pAnimationChannels[i])
				delete pAnimationChannels[i];
		}

		pAnimationChannels.clear();

		pFileManager->Close();
		return false;
	}

	pFileManager->Close();
	return true;
}

bool CAssetManager::LoadGeometry( const char *sFileName, std::vector<SVertex> &verVertices, std::vector<unsigned int> &uiIndices )
{
	if (!pFileManager->Open( sFileName, EFileType::t_geometry ))
		return false;

	if (!LoadGeometryInternal( verVertices, uiIndices ))
	{
		verVertices.clear();
		uiIndices.clear();

		pFileManager->Close();
		return false;
	}

	pFileManager->Close();
	return true;
}

bool CAssetManager::LoadImage( const char *sFileName, std::vector<unsigned char *> &pData, std::vector<unsigned int> &uiWidth, std::vector<unsigned int> &uiHeight, std::vector<unsigned int> &uiChannels )
{
	if (!pFileManager->Open( sFileName, EFileType::t_image ))
		return false;

	if (!LoadImageInternal( pData, uiWidth, uiHeight, uiChannels ))
	{
		for (unsigned short i = 0; i < (unsigned short)pData.size(); i++)
			delete[] pData[i];

		pData.clear();
		uiWidth.clear();
		uiHeight.clear();
		uiChannels.clear();

		pFileManager->Close();
		return false;
	}

	pFileManager->Close();
	return true;
}

bool CAssetManager::LoadSkeleton( const char *sFileName, SSkeletonNode *&pSkeletonNode, unsigned int &uiSkeletonBonesCount )
{
	if (!pFileManager->Open( sFileName, EFileType::t_skeleton ))
		return false;

	if (!LoadSkeletonInternal( pSkeletonNode, uiSkeletonBonesCount ))
	{
		pFileManager->Close();
		return false;
	}

	pFileManager->Close();
}

bool CAssetManager::LoadConfigInternal( char *&sCommand )
{
	unsigned int uiBytes = pFileManager->GetBytes();

	sCommand = new char[uiBytes + 1];

	if (!pFileManager->Read( sCommand, uiBytes ))
	{
		delete[] sCommand;
		return false;
	}

	sCommand[uiBytes] = '\0';

	return true;
}

bool CAssetManager::LoadShaderInternal( char *&sSource )
{
	unsigned int uiBytes = pFileManager->GetBytes();

	char *sContents = new char[uiBytes + 1];

	if (!pFileManager->Read( sContents, uiBytes ))
	{
		delete[] sContents;
		return false;
	}

	sContents[uiBytes] = '\0';

	const char *sReadChar = sContents;

	std::vector<char> cSource;

	while (*sReadChar)
	{
		if (*sReadChar == '#')
		{
			const char *sPreProcessorChar = sReadChar + 1;
			int iPreProcessorSize = (int)(UTIL_strchr( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);
			if (UTIL_strcmp( "include", sPreProcessorChar, UTIL_Min( sizeof( "include" ), iPreProcessorSize ) - 1 ) == 0)
			{
				sPreProcessorChar = UTIL_strchr( sPreProcessorChar + iPreProcessorSize, '\"' ) + 1;
				int iPathSize = (int)(UTIL_strchr( sPreProcessorChar, '\"' ) - sPreProcessorChar);

				char *sPathName = new char[iPathSize + 1];
				UTIL_strncpy( sPathName, sPreProcessorChar, iPathSize );
				sPathName[iPathSize] = '\0';

				char *sIncludeSource;

				if (!pFileManager->Open( sPathName ))
				{
					delete[] sPathName;
					delete[] sContents;

					return false;
				}

				if (!LoadShaderInternal( sIncludeSource ))
				{
					delete[] sPathName;
					delete[] sContents;

					pFileManager->Close();
					return false;
				}

				pFileManager->Close();

				const char *sIncludeSourceReadChar = sIncludeSource;
				while (*sIncludeSourceReadChar)
					cSource.push_back( *sIncludeSourceReadChar++ );

				delete[] sIncludeSource;

				sReadChar = sPreProcessorChar + iPathSize + 2;
				cSource.push_back( '\n' );

				delete[] sPathName;
				continue;
			}
		}

		cSource.push_back( *sReadChar++ );
	}

	delete[] sContents;

	unsigned short usSize = (unsigned short)cSource.size();
	sSource = new char[usSize + 1];
	for (unsigned int i = 0; i < usSize; i++)
		sSource[i] = cSource[i];

	sSource[usSize] = '\0';

	return true;
}

bool CAssetManager::LoadAnimationInternal( float &flTime, std::vector<SAnimationChannel *> &pAnimationChannels )
{
	if (!pFileManager->Read( flTime ))
		return false;

	unsigned int uiAnimationChannelSlots;
	if (!pFileManager->Read( uiAnimationChannelSlots ))
		return false;

	unsigned int uiAnimationChannelCount;
	if (!pFileManager->Read( uiAnimationChannelCount ))
		return false;

	pAnimationChannels.resize( uiAnimationChannelSlots, NULL );

	for (unsigned int i = 0; i < uiAnimationChannelCount; i++)
	{
		unsigned int uiAnimationChannelSlot;
		if (!pFileManager->Read( uiAnimationChannelSlot ))
			return false;

		SAnimationChannel *pAnimationChannel = new SAnimationChannel;
		pAnimationChannels[uiAnimationChannelSlot] = pAnimationChannel;

		unsigned int uiPositionsCount;
		if (!pFileManager->Read( uiPositionsCount ))
			return false;

		pAnimationChannel->flPositionTimes.resize( uiPositionsCount );
		pAnimationChannel->vec3Positions.resize( uiPositionsCount );

		for (unsigned int j = 0; j < uiPositionsCount; j++)
		{
			if (!pFileManager->Read( pAnimationChannel->flPositionTimes[j] ) || !pFileManager->Read( pAnimationChannel->vec3Positions[j] ))
				return false;
		}

		unsigned int uiRotationsCount;
		if (!pFileManager->Read( uiRotationsCount ))
			return false;

		pAnimationChannel->flRotationTimes.resize( uiRotationsCount );
		pAnimationChannel->qRotations.resize( uiRotationsCount );

		for (unsigned int j = 0; j < uiRotationsCount; j++)
		{
			if (!pFileManager->Read( pAnimationChannel->flRotationTimes[j] ) || !pFileManager->Read( pAnimationChannel->qRotations[j].w ) || !pFileManager->Read( pAnimationChannel->qRotations[j].x ) || !pFileManager->Read( pAnimationChannel->qRotations[j].y ) || !pFileManager->Read( pAnimationChannel->qRotations[j].z ))
				return false;
		}

		unsigned int uiScalesCount;
		if (!pFileManager->Read( uiScalesCount ))
			return false;

		pAnimationChannel->flScaleTimes.resize( uiScalesCount );
		pAnimationChannel->vec3Scales.resize( uiScalesCount );

		for (unsigned int j = 0; j < uiScalesCount; j++)
		{
			if (!pFileManager->Read( pAnimationChannel->flScaleTimes[j] ) || !pFileManager->Read( pAnimationChannel->vec3Scales[j] ))
				return false;
		}
	}

	return true;
}

bool CAssetManager::LoadGeometryInternal( std::vector<SVertex> &verVertices, std::vector<unsigned int> &uiIndices )
{
	unsigned int uiVertexCount;
	if (!pFileManager->Read( uiVertexCount ))
		return false;

	for (unsigned int i = 0; i < uiVertexCount; i++)
	{
		SVertex verVertex;
		unsigned int uiNumAffectingBones;
		if (!pFileManager->Read( verVertex.vec3Position ) || !pFileManager->Read( verVertex.vec3Normal ) || !pFileManager->Read( verVertex.vec3Tangent ) || !pFileManager->Read( verVertex.vec3Bitangent ) || !pFileManager->Read( verVertex.vec2TexCoords ) || !pFileManager->Read( uiNumAffectingBones ))
			return false;

		for (unsigned int j = 0; j < uiNumAffectingBones; j++)
		{
			if (!pFileManager->Read( verVertex.vec4BoneIDs[j] ) || !pFileManager->Read( verVertex.vec4Weights[j] ))
				return false;
		}

		verVertices.push_back( verVertex );
	}

	unsigned int uiFacesCount;
	if (!pFileManager->Read( uiFacesCount ))
		return false;

	for (unsigned int i = 0; i < uiFacesCount; i++)
	{
		unsigned int uiIndicesCount;
		if (!pFileManager->Read( uiIndicesCount ))
			return false;

		for (unsigned int j = 0; j < uiIndicesCount; j++)
		{
			unsigned int uiIndex;
			if (!pFileManager->Read( uiIndex ))
				return false;

			uiIndices.push_back( uiIndex );
		}
	}

	return true;
}

bool CAssetManager::LoadImageInternal( std::vector<unsigned char *> &pData, std::vector<unsigned int> &uiWidth, std::vector<unsigned int> &uiHeight, std::vector<unsigned int> &uiChannels )
{
	unsigned int uiImageCount;
	if (!pFileManager->Read( uiImageCount ))
		return false;

	for (unsigned int i = 0; i < uiImageCount; i++)
	{
		unsigned int uiNewWidth;
		if (!pFileManager->Read( uiNewWidth ))
			return false;

		unsigned int uiNewHeight;
		if (!pFileManager->Read( uiNewHeight ))
			return false;

		unsigned int uiNewChannels;
		if (!pFileManager->Read( uiNewChannels ))
			return false;

		unsigned int uiSize = uiNewWidth * uiNewHeight * uiNewChannels;
		unsigned char *pNewData = new unsigned char[uiSize];
		if (!pFileManager->Read( pNewData, uiSize ))
		{
			delete[] pNewData;
			return false;
		}

		pData.push_back( pNewData );
		uiWidth.push_back( uiNewWidth );
		uiHeight.push_back( uiNewHeight );
		uiChannels.push_back( uiNewChannels );
	}

	return true;
}

bool CAssetManager::LoadSkeletonInternal( SSkeletonNode *&pSkeletonNode, unsigned int &uiSkeletonBonesCount )
{
	std::vector<SSkeletonBone *> pSkeletonBones;
	std::vector<SSkeletonNode *> pSkeletonNodes;

	if (!pFileManager->Read( uiSkeletonBonesCount ))
		return false;

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

			return false;
		}
	}

	unsigned int uiSkeletonNodesCount;
	if (!pFileManager->Read( uiSkeletonNodesCount ))
	{
		for (unsigned int i = 0; i < uiSkeletonBonesCount; i++)
			delete pSkeletonBones[i];

		return false;
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

			return false;
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

				return false;
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

				return false;
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

			return false;
		}

		pChildren[i].resize( uiChildrenCount );

		for (unsigned int j = 0; j < uiChildrenCount; j++)
		{
			unsigned int uiChildIndex;
			if (!pFileManager->Read( uiChildIndex ) || i == uiChildIndex)
			{
				for (unsigned int k = 0; k < uiSkeletonNodesCount; k++)
					delete pSkeletonNodes[k];

				return false;
			}

			pChildren[i][j] = pSkeletonNodes[uiChildIndex];
		}
	}

	for (unsigned int i = 0; i < uiSkeletonNodesCount; i++)
		pSkeletonNodes[i]->pChildren = pChildren[i];

	pSkeletonNode = pSkeletonNodes[0];

	return true;
}