#include "Global.h"
#include "FileManager.h"
#include "AnimationResourceData.h"
#include "CubemapResourceData.h"
#include "FontResourceData.h"
#include "GeometryResourceData.h"
#include "SkeletonResourceData.h"
#include "TextureResourceData.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <string>
#include <fstream>

bool g_bDebug = false;
bool g_bHelp = false;
const char *g_sFilePath = NULL;
const char *g_sRootPath = "";

static inline glm::mat4 mat4_cast( const aiMatrix4x4 &m ) { return glm::transpose( glm::make_mat4( &m.a1 ) ); }

// TODO: verify, optimise, clean, organise these alphabetically 

// TODO: make the width and height of the bitmap be automatically generated based on font size or something idk
bool UTIL_ProcessFont( CTextBlock *pTextBlock )
{
	SFontResourceData sFontData;

	const char *sFontName;
	if (!pTextBlock->GetValue( sFontName, 1, "fontname" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: fontname not found.\n";

		return false;
	}

	if (!pTextBlock->GetValue( sFontData.uiFontSize, 1, "fontsize" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: fontsize not found.\n";

		return false;
	}

	if (!pTextBlock->GetValue( sFontData.vec2BitmapSize.x, 1, "width" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: width not found.\n";

		return false;
	}

	int iPadding;
	if (!pTextBlock->GetValue( iPadding, 1, "padding" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: padding not found.\n";

		return false;
	}

	FT_Error ftError;

	FT_Library ftLib;
	ftError = FT_Init_FreeType( &ftLib );
	if (ftError)
	{
		std::cout << "ERROR: FT_Init_FreeType failed, error code " << ftError << '\n';
		return false;
	}

	char *sFullFontPath = UTIL_stradd( g_sRootPath, sFontName );
	FT_Face ftFace;
	ftError = FT_New_Face( ftLib, sFullFontPath, 0, &ftFace );
	delete[] sFullFontPath;
	if (ftError)
	{
		std::cout << "ERROR: failed to open file " << sFontName << ", error code " << ftError << '\n';
		return false;
	}

	ftError = FT_Set_Pixel_Sizes( ftFace, 0, sFontData.uiFontSize );
	if (ftError)
	{
		std::cout << "ERROR: failed to set font size, error code " << ftError << '\n';
		return false;
	}

	int iXPos = 0;
	int iYPos = 0;
	int iLineMaxHeight = 0;
	int iMaxDepth = INT_MIN;
	
	for (unsigned int i = 0; i < FONT_CHAR_SIZE; i++)
	{
		FT_UInt ftGlyphIndex = FT_Get_Char_Index( ftFace, i );
		ftError = FT_Load_Glyph( ftFace, ftGlyphIndex, FT_LOAD_DEFAULT );
		if (ftError)
		{
			std::cout << "ERROR: failed to load glyph, error code " << ftError << '\n';
			return false;
		}

		const FT_Glyph_Metrics &ftGlyphMetrics = ftFace->glyph->metrics;

		int iCurrentXPos = iXPos;
		int iCurrentYPos = iYPos;

		int iWidth = ftGlyphMetrics.width / 64;
		int iHeight = ftGlyphMetrics.height / 64;

		int iOffsetX = ftGlyphMetrics.horiBearingX / 64;
		int iOffsetY = -ftGlyphMetrics.horiBearingY / 64;

		int iAdvance = ftGlyphMetrics.horiAdvance / 64;

		iXPos += iWidth + iPadding;
		if (iXPos > sFontData.vec2BitmapSize.x)
		{
			iXPos = 0;
			iYPos += iLineMaxHeight + iPadding;
			iLineMaxHeight = 0;

			iCurrentXPos = iXPos;
			iCurrentYPos = iYPos;

			iXPos += iWidth;
		}

		if (iLineMaxHeight < iHeight)
			iLineMaxHeight = iHeight;

		int iDepth = iOffsetY + iHeight;
		if (iMaxDepth < iDepth)
			iMaxDepth = iDepth;

		SChar &cChar = sFontData.sChars[i];
		cChar.vec2Position = glm::ivec2( iCurrentXPos, iCurrentYPos );
		cChar.vec2Size = glm::ivec2( iWidth, iHeight );
		cChar.vec2Offset = glm::ivec2( iOffsetX, iOffsetY );
		cChar.iAdvance = iAdvance;
	}

	for (unsigned int i = 0; i < FONT_CHAR_SIZE; i++)
	{
		SChar &cChar = sFontData.sChars[i];
		cChar.vec2Offset.y -= iMaxDepth;
	}

	sFontData.vec2BitmapSize.y = iYPos + iLineMaxHeight;

	sFontData.pBitmapData = new unsigned char[sFontData.vec2BitmapSize.x * sFontData.vec2BitmapSize.y];
	memset( sFontData.pBitmapData, 0, sFontData.vec2BitmapSize.x * sFontData.vec2BitmapSize.y );

	for (unsigned int i = 0; i < FONT_CHAR_SIZE; i++)
	{
		FT_UInt ftGlyphIndex = FT_Get_Char_Index( ftFace, i );
		ftError = FT_Load_Glyph( ftFace, ftGlyphIndex, FT_LOAD_DEFAULT );
		if (ftError)
		{
			std::cout << "ERROR: failed to load glyph, error code " << ftError << '\n';
			return false;
		}

		ftError = FT_Render_Glyph( ftFace->glyph, FT_RENDER_MODE_NORMAL );
		if (ftError)
		{
			std::cout << "ERROR: failed to render glyph, error code: " << ftError << '\n';
		}

		const FT_Bitmap &bitmap = ftFace->glyph->bitmap;

		const SChar &cChar = sFontData.sChars[i];

		for (unsigned int x = 0; x < bitmap.width; x++)
		{
			for (unsigned int y = 0; y < bitmap.rows; y++)
				sFontData.pBitmapData[(cChar.vec2Position.y + y) * sFontData.vec2BitmapSize.x + (cChar.vec2Position.x + x)] = bitmap.buffer[(y * (bitmap.width) + x)];
		}
	}

	const char *sName;
	if (!pTextBlock->GetValue( sName, 1, "name" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: name not found.\n";

		return false;
	}

	if (!pFileManager->Open( sName, EFileType::t_font ))
	{
		if (g_bDebug)
			std::cout << "ERROR: couldn't open font file with name " << sName << "\n";

		return false;
	}

	if (!pFileManager->Write( sFontData ))
	{
		if (g_bDebug)
			std::cout << "ERROR: couldn't write font data with name " << sName << "\n";

		return false;
	}

	FT_Done_FreeType( ftLib );

	pFileManager->Close();

	return true;
}

bool UTIL_ProcessCubemap( CTextBlock *pTextBlock )
{
	SCubemapResourceData sCubemapData;

	if (!pTextBlock->GetValue( sCubemapData.bFiltered, 1, "filtered" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: filtered not found.\n";

		return false;
	}

	const char *sImage[6];
	if (!pTextBlock->GetValue( sImage[0], 1, "right" ) )
	{
		if (g_bDebug)
			std::cout << "ERROR: right not found.\n";

		return false;
	}

	if (!pTextBlock->GetValue( sImage[1], 1, "left" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: left not found.\n";

		return false;
	}

	if (!pTextBlock->GetValue( sImage[2], 1, "top" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: front not top.\n";

		return false;
	}

	if (!pTextBlock->GetValue( sImage[3], 1, "bottom" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: bottom not found.\n";

		return false;
	}

	if (!pTextBlock->GetValue( sImage[4], 1, "front" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: front not found.\n";

		return false;
	}

	if (!pTextBlock->GetValue( sImage[5], 1, "back" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: back not found.\n";

		return false;
	}

	for (unsigned int i = 0; i < 6; i++)
	{
		char *sFullImagePath = UTIL_stradd( g_sRootPath, sImage[i] );
		sCubemapData.pData[i] = stbi_load( sFullImagePath, &sCubemapData.iWidth[i], &sCubemapData.iHeight[i], (int *)(&sCubemapData.ucChannels[i]), 0 );
		delete[] sFullImagePath;

		if (!sCubemapData.pData[i])
		{
			if (g_bDebug)
				std::cout << "ERROR: couldn't open image file with name " << sImage << "\n";

			for (unsigned int j = 0; j < i; j++)
				stbi_image_free( sCubemapData.pData[j] );

			return false;
		}
	}

	const char *sName;
	if (!pTextBlock->GetValue( sName, 1, "name" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: name not found.\n";

		for (unsigned int i = 0; i < 6; i++)
			stbi_image_free( sCubemapData.pData[i] );

		return false;
	}

	if (!pFileManager->Open( sName, EFileType::t_cubemap ))
	{
		if (g_bDebug)
			std::cout << "ERROR: couldn't open cubemap file with name " << sName << "\n";

		for (unsigned int i = 0; i < 6; i++)
			stbi_image_free( sCubemapData.pData[i] );

		return false;
	}

	if (!pFileManager->Write( sCubemapData ))
	{
		if (g_bDebug)
			std::cout << "ERROR: couldn't write cubemap data with name " << sName << "\n";

		for (unsigned int i = 0; i < 6; i++)
			stbi_image_free( sCubemapData.pData[i] );

		return false;
	}

	for (unsigned int i = 0; i < 6; i++)
		stbi_image_free( sCubemapData.pData[i] );

	pFileManager->Close();

	return true;
}

bool UTIL_ProcessGeometry( const char *sName, const aiScene *pScene, std::vector<glm::mat4> &matBoneOffsets, std::unordered_map<std::string, unsigned int> &uiBoneMapping )
{
	for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
	{
		aiMesh *pMesh = pScene->mMeshes[i];

		if (pMesh->HasBones())
		{
			for (unsigned int i = 0; i < pMesh->mNumBones; i++)
			{
				std::string sBoneName( pMesh->mBones[i]->mName.data );
				if (uiBoneMapping.find( sBoneName ) == uiBoneMapping.end())
				{
					uiBoneMapping[sBoneName] = (unsigned int)matBoneOffsets.size();
					matBoneOffsets.push_back( mat4_cast( pMesh->mBones[i]->mOffsetMatrix ) );
				}
			}
		}

		const char *sGeometryName;
		char *sNumberedGeometryName = NULL;
		if (!pMesh->mName.C_Str() || !*pMesh->mName.C_Str())
		{
			if (pScene->mNumMeshes == 1)
			{
				sGeometryName = sName;
			}
			else
			{
				char sIndex[33];
				itoa( i, sIndex, 10 );
				sNumberedGeometryName = UTIL_stradd( sName, sIndex );
				sGeometryName = sNumberedGeometryName;
			}
		}
		else
		{
			sGeometryName = pMesh->mName.C_Str();
		}

		if (!pFileManager->Open( sGeometryName, EFileType::t_geometry ))
		{
			if (g_bDebug)
				std::cout << "ERROR: couldn't open geometry file with name " << sGeometryName << "\n";

			if (sNumberedGeometryName)
				delete[] sNumberedGeometryName;

			return false;
		}

		delete[] sNumberedGeometryName;

		SGeometryResourceData sGeometryData;

		sGeometryData.verVertices.resize( pMesh->mNumVertices );

		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			SVertex3D &verVertex = sGeometryData.verVertices[i];
			verVertex.vec3Position.x = pMesh->mVertices[i].x;
			verVertex.vec3Position.y = pMesh->mVertices[i].y;
			verVertex.vec3Position.z = pMesh->mVertices[i].z;
			verVertex.vec3Normal.x = pMesh->mNormals[i].x;
			verVertex.vec3Normal.y = pMesh->mNormals[i].y;
			verVertex.vec3Normal.z = pMesh->mNormals[i].z;
			verVertex.vec3Tangent.x = pMesh->mTangents[i].x;
			verVertex.vec3Tangent.y = pMesh->mTangents[i].y;
			verVertex.vec3Tangent.z = pMesh->mTangents[i].z;
			verVertex.vec3Bitangent.x = pMesh->mBitangents[i].x;
			verVertex.vec3Bitangent.y = pMesh->mBitangents[i].y;
			verVertex.vec3Bitangent.z = pMesh->mBitangents[i].z;
			verVertex.vec2TexCoords.x = pMesh->mTextureCoords[0][i].x;
			verVertex.vec2TexCoords.y = pMesh->mTextureCoords[0][i].y;

			verVertex.vec4BoneIDs = g_ivec4Zero;
			verVertex.vec4Weights = g_vec4Zero;

			if (pMesh->HasBones())
			{
				std::vector<unsigned int> uiBoneIDs;
				std::vector<float> flWeights;

				for (unsigned int j = 0; j < pMesh->mNumBones; j++)
				{
					unsigned int uiNumWeights = pMesh->mBones[j]->mNumWeights;
					aiVertexWeight *pWeights = pMesh->mBones[j]->mWeights;
					for (unsigned int k = 0; k < uiNumWeights; k++)
					{
						if (pWeights[k].mVertexId == i)
						{
							std::string sBoneName( pMesh->mBones[j]->mName.data );
							uiBoneIDs.push_back( uiBoneMapping[sBoneName] );
							flWeights.push_back( pWeights[k].mWeight );
							break;
						}
					}

					if ((unsigned int)uiBoneIDs.size() >= 4)
						break;
				}

				unsigned int uiNumAffectingBones = UTIL_min( (unsigned int)uiBoneIDs.size(), (unsigned int)4 );
				for (unsigned int j = 0; j < uiNumAffectingBones; j++)
				{
					verVertex.vec4BoneIDs[j] = uiBoneIDs[j];
					verVertex.vec4Weights[j] = flWeights[j];
				}
			}
		}

		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const aiFace &face = pMesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				sGeometryData.uiIndices.push_back( face.mIndices[j] );
		}

		if (!pFileManager->Write( sGeometryData ))
		{
			if (g_bDebug)
				std::cout << "ERROR: couldn't write geometry data with name " << sGeometryName << "\n";

			return false;
		}

		pFileManager->Close();
	}

	return true;
}

void UTIL_ProcessSkeletonInternal( aiNode *pNode, SSkeletonNode *&pData, std::vector<glm::mat4> &matBoneOffsets, std::unordered_map<std::string, unsigned int> uiBoneMapping, unsigned int &uiNodeCount, std::unordered_map<std::string, unsigned int> &uiNodeMapping )
{
	pData = new SSkeletonNode;

	std::string sNodeName = pNode->mName.C_Str();

	pData->uiIndex = uiNodeCount++;
	if (!sNodeName.empty())
		uiNodeMapping[sNodeName] = pData->uiIndex;

	bool bHasSkeletonBone = uiBoneMapping.find( sNodeName ) != uiBoneMapping.end();

	if (bHasSkeletonBone)
	{
		pData->pSkeletonBone = new SSkeletonBone;
		pData->pSkeletonBone->uiIndex = uiBoneMapping[sNodeName];
		pData->pSkeletonBone->matOffset = matBoneOffsets[pData->pSkeletonBone->uiIndex];
	}

	pData->pChildren.resize( pNode->mNumChildren );
	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
		UTIL_ProcessSkeletonInternal( pNode->mChildren[i], pData->pChildren[i], matBoneOffsets, uiBoneMapping, uiNodeCount, uiNodeMapping );
}

bool UTIL_ProcessSkeleton( const char *sName, const aiScene *pScene, std::vector<glm::mat4> &matBoneOffsets, std::unordered_map<std::string, unsigned int> uiBoneMapping, unsigned int &uiNodeCount, std::unordered_map<std::string, unsigned int> &uiNodeMapping )
{
	if ((unsigned int)matBoneOffsets.size() == 0)
		return true;

	if (!pFileManager->Open( sName, EFileType::t_skeleton ))
	{
		if (g_bDebug)
			std::cout << "ERROR: couldn't open skeleton file with name " << sName << "\n";

		return false;
	}

	SSkeletonResourceData sSkeletonData;

	sSkeletonData.uiSkeletonBonesCount = (unsigned int)matBoneOffsets.size();

	uiNodeCount = 0;
	UTIL_ProcessSkeletonInternal( pScene->mRootNode, sSkeletonData.pSkeletonNode, matBoneOffsets, uiBoneMapping, uiNodeCount, uiNodeMapping );

	if (!pFileManager->Write( sSkeletonData ))
	{
		if (g_bDebug)
			std::cout << "ERROR: couldn't write skeleton data with name " << sName << "\n";

		return false;
	}

	pFileManager->Close();

	return true;
}

bool UTIL_ProcessAnimation( const char *sName, const aiScene *pScene, unsigned int uiNodeCount, std::unordered_map<std::string, unsigned int> &uiNodeMapping )
{
	for (unsigned int i = 0; i < pScene->mNumAnimations; i++)
	{
		aiAnimation *pAnimation = pScene->mAnimations[i];

		const char *sAnimationName;
		char *sNumberedAnimationName = NULL;
		if (!pAnimation->mName.C_Str() || !*pAnimation->mName.C_Str())
		{
			if (pScene->mNumAnimations == 1)
			{
				sAnimationName = sName;
			}
			else
			{
				char sIndex[33];
				itoa( i, sIndex, 10 );
				sNumberedAnimationName = UTIL_stradd( sName, sIndex );
				sAnimationName = sNumberedAnimationName;
			}
		}
		else
		{
			sAnimationName = pAnimation->mName.C_Str();
		}

		if (!pFileManager->Open( sAnimationName, EFileType::t_animation ))
		{
			if (g_bDebug)
				std::cout << "ERROR: couldn't open animation file with name " << sAnimationName << "\n";

			if (sNumberedAnimationName)
				delete[] sNumberedAnimationName;

			return false;
		}

		if (sNumberedAnimationName)
			delete[] sNumberedAnimationName;

		SAnimationResourceData sAnimationData;

		sAnimationData.flTime = (float)pAnimation->mDuration / (float)pAnimation->mTicksPerSecond;

		sAnimationData.pAnimationChannels.resize( uiNodeCount, NULL );

		for (unsigned int j = 0; j < pAnimation->mNumChannels; j++)
		{
			aiNodeAnim *pNodeAnim = pAnimation->mChannels[j];
			std::string sNodeName = pNodeAnim->mNodeName.C_Str();

			if (uiNodeMapping.find( sNodeName ) == uiNodeMapping.end())
			{
				if (g_bDebug)
					std::cout << "WARNING: Cannot find associated node '" << sNodeName << "' in animation node!\n";

				continue;
			}

			SAnimationChannel *pAnimationChannel = new SAnimationChannel;
			sAnimationData.pAnimationChannels[uiNodeMapping[sNodeName]] = pAnimationChannel;

			pAnimationChannel->flPositionTimes.resize( pNodeAnim->mNumPositionKeys );
			pAnimationChannel->vec3Positions.resize( pNodeAnim->mNumPositionKeys );
			for (unsigned int k = 0; k < pNodeAnim->mNumPositionKeys; k++)
			{
				pAnimationChannel->flPositionTimes[k] = (float)pNodeAnim->mPositionKeys[k].mTime / (float)pAnimation->mTicksPerSecond;
				pAnimationChannel->vec3Positions[k].x = pNodeAnim->mPositionKeys[k].mValue.x;
				pAnimationChannel->vec3Positions[k].y = pNodeAnim->mPositionKeys[k].mValue.y;
				pAnimationChannel->vec3Positions[k].z = pNodeAnim->mPositionKeys[k].mValue.z;
			}

			pAnimationChannel->flRotationTimes.resize( pNodeAnim->mNumRotationKeys );
			pAnimationChannel->qRotations.resize( pNodeAnim->mNumRotationKeys );
			for (unsigned int k = 0; k < pNodeAnim->mNumRotationKeys; k++)
			{
				pAnimationChannel->flRotationTimes[k] = (float)pNodeAnim->mRotationKeys[k].mTime / (float)pAnimation->mTicksPerSecond;
				pAnimationChannel->qRotations[k].w = pNodeAnim->mRotationKeys[k].mValue.w;
				pAnimationChannel->qRotations[k].x = pNodeAnim->mRotationKeys[k].mValue.x;
				pAnimationChannel->qRotations[k].y = pNodeAnim->mRotationKeys[k].mValue.y;
				pAnimationChannel->qRotations[k].z = pNodeAnim->mRotationKeys[k].mValue.z;
			}

			pAnimationChannel->flScaleTimes.resize( pNodeAnim->mNumScalingKeys );
			pAnimationChannel->vec3Scales.resize( pNodeAnim->mNumScalingKeys );
			for (unsigned int k = 0; k < pNodeAnim->mNumScalingKeys; k++)
			{
				pAnimationChannel->flScaleTimes[k] = (float)pNodeAnim->mScalingKeys[k].mTime / (float)pAnimation->mTicksPerSecond;
				pAnimationChannel->vec3Scales[k].x = pNodeAnim->mScalingKeys[k].mValue.x;
				pAnimationChannel->vec3Scales[k].y = pNodeAnim->mScalingKeys[k].mValue.y;
				pAnimationChannel->vec3Scales[k].z = pNodeAnim->mScalingKeys[k].mValue.z;
			}
		}

		if (!pFileManager->Write( sAnimationData ))
		{
			if (g_bDebug)
				std::cout << "ERROR: couldn't write animation data with name " << sAnimationName << "\n";

			for (unsigned int j = 0; j < sAnimationData.pAnimationChannels.size(); j++)
			{
				if (sAnimationData.pAnimationChannels[j])
					delete sAnimationData.pAnimationChannels[j];
			}

			return false;
		}

		for (unsigned int j = 0; j < sAnimationData.pAnimationChannels.size(); j++)
		{
			if (sAnimationData.pAnimationChannels[j])
				delete sAnimationData.pAnimationChannels[j];
		}

		pFileManager->Close();
	}

	return true;
}

bool UTIL_ProcessModel( CTextBlock *pTextBlock )
{
	const char *sName;
	if (!pTextBlock->GetValue( sName, 1, "name" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: name not found.\n";

		return false;
	}

	bool bFlipUV;
	if (!pTextBlock->GetValue( bFlipUV, 1, "flipuv" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: flipuv not found.\n";

		return false;
	}

	const char *sModel;
	if (!pTextBlock->GetValue( sModel, 1, "model" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: model not found.\n";

		return false;
	}

	char *sModelPath = UTIL_stradd( g_sRootPath, sModel );
	Assimp::Importer imImporter;
	const aiScene *pScene = imImporter.ReadFile( sModelPath, bFlipUV ? (aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs) : (aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenSmoothNormals) );
	delete[] sModelPath;

	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
	{
		if (g_bDebug)
			std::cout << "ERROR: assimp error: " << imImporter.GetErrorString() << '\n';

		return false;
	}

	std::vector<glm::mat4> matBoneOffsets;
	std::unordered_map<std::string, unsigned int> uiBoneMapping;

	if (!UTIL_ProcessGeometry( sName, pScene, matBoneOffsets, uiBoneMapping ))
	{
		if (g_bDebug)
			std::cout << "ERROR: failed to process geometry.\n";

		return false;
	}

	unsigned int uiNodeCount;
	std::unordered_map<std::string, unsigned int> uiNodeMapping;
	if (!UTIL_ProcessSkeleton( sName, pScene, matBoneOffsets, uiBoneMapping, uiNodeCount, uiNodeMapping ))
	{
		if (g_bDebug)
			std::cout << "ERROR: failed to process skeleton.\n";

		return false;
	}

	if (!UTIL_ProcessAnimation( sName, pScene, uiNodeCount, uiNodeMapping ))
	{
		if (g_bDebug)
			std::cout << "ERROR: failed to process animation.\n";

		return false;
	}

	return true;
}

bool UTIL_ProcessTexture( CTextBlock *pTextBlock )
{
	STextureResourceData sTextureData;

	if (!pTextBlock->GetValue( sTextureData.bFiltered, 1, "filtered" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: filtered not found.\n";

		return false;
	}

	if (!pTextBlock->GetValue( sTextureData.eTextureWrap, 1, "texturewrap" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: texturewrap not found.\n";

		return false;
	}

	if (sTextureData.eTextureWrap == ETextureWrap::t_border)
	{
		if (!pTextBlock->GetValue( sTextureData.vec4BorderColor, 1, "bordercolor" ))
		{
			if (g_bDebug)
				std::cout << "ERROR: bordercolor not found.\n";

			return false;
		}
	}

	const char *sImage;
	if (!pTextBlock->GetValue( sImage, 1, "image" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: image not found.\n";

		return false;
	}

	char *sFullImagePath = UTIL_stradd( g_sRootPath, sImage );
	sTextureData.pData = stbi_load( sFullImagePath, &sTextureData.iWidth, &sTextureData.iHeight, (int *)(&sTextureData.ucChannels), 0 );
	delete[] sFullImagePath;

	if (!sTextureData.pData)
	{
		if (g_bDebug)
			std::cout << "ERROR: couldn't open image file with name " << sImage << "\n";

		return false;
	}

	const char *sName;
	if (!pTextBlock->GetValue( sName, 1, "name" ))
	{
		if (g_bDebug)
			std::cout << "ERROR: name not found.\n";

		stbi_image_free( sTextureData.pData );

		return false;
	}

	if (!pFileManager->Open( sName, EFileType::t_texture ))
	{
		if (g_bDebug)
			std::cout << "ERROR: couldn't open texture file with name " << sName << "\n";

		stbi_image_free( sTextureData.pData );

		return false;
	}

	if (!pFileManager->Write( sTextureData ))
	{
		if (g_bDebug)
			std::cout << "ERROR: couldn't write texture data with name " << sName << "\n";

		stbi_image_free( sTextureData.pData );

		return false;
	}

	stbi_image_free( sTextureData.pData );

	pFileManager->Close();

	return true;
}

int main( int iArgC, const char *sArgV[] )
{
	CreateGlobals();
	for (unsigned int i = 1; i < (unsigned int)iArgC; i++)
	{
		const char *sArgument = sArgV[i];
		if (*sArgument == '-')
		{
			sArgument++;
			if (!g_bDebug && UTIL_streq( sArgument, "debug" ))
				g_bDebug = true;
			else if (!g_bHelp && UTIL_streq( sArgument, "help" ))
				g_bHelp = true;
			else
				std::cout << "ERROR: invalid parameter -" << sArgument << ", use parameter -help for assistance.\n";
		}
		else if (!g_sFilePath)
		{
			g_sFilePath = sArgument;
		}
	}

	g_bDebug = true;

	if (g_bHelp) // TODO: fix this, innacurate
		std::cout << "INFO: 3DRAsset is a tool used for compiling assets for 3DREngine-based programs.\nTo use this program, run it with a source file as an argument. If the source file is of a proper format, this program will output the associated compiled file(s) with the proper file extension(s).\nParameters for this program are:\n-debug, which provides a detailed program output;\n-help, which display this information;\n-nomaterial, which prevents the compilation of materials;\n-nomaterialcontroller, which prevents the compilation of material controllers;\nFor information on how to format source files, please follow the instructions located in the reademe.\n";

	if (g_sFilePath)
	{
		if (!pFileManager->Open( g_sFilePath, std::ios::in ))
			return EXIT_FAILURE;

		char *sBuffer;
		if (!pFileManager->Buffer( sBuffer ))
			return EXIT_FAILURE;

		pFileManager->Close();

		CTextReader trTextReader;
		bool bResult = trTextReader.ReadText( sBuffer );
		delete[] sBuffer;

		if (!bResult)
			return EXIT_FAILURE;

		char *sRootPath = NULL;
		const char *sChar = UTIL_strpbrkl( g_sFilePath, "/\\" );
		if (sChar)
		{
			sRootPath = UTIL_strndup( g_sFilePath, (unsigned int)(sChar - g_sFilePath + 1) );
			g_sRootPath = sRootPath;
		}

		CTextBlock *pTextBlock = trTextReader.GetTextBlock();

		for (unsigned int i = 0; i < pTextBlock->GetTextLineCount(); i++)
		{
			CTextLine *pTextLine = pTextBlock->GetTextLine( i );

			CTextBlock *pResourceTextBlock;
			if (!pTextLine->GetValue( pResourceTextBlock, 1 ))
			{
				if (g_bDebug)
					std::cout << "ERROR: failed to interpret resource text block.\n";

				return EXIT_FAILURE;
			}

			if (pTextLine->IsKey( "cubemap" ))
			{
				if (!UTIL_ProcessCubemap( pResourceTextBlock ))
				{
					if (g_bDebug)
						std::cout << "ERROR: failed to process cubemap.\n";

					return EXIT_FAILURE;
				}
			}
			else if (pTextLine->IsKey( "font" ))
			{
				if (!UTIL_ProcessFont( pResourceTextBlock ))
				{
					if (g_bDebug)
						std::cout << "ERROR: failed to process model.\n";

					return EXIT_FAILURE;
				}
			}
			else if (pTextLine->IsKey( "model" ))
			{
				if (!UTIL_ProcessModel( pResourceTextBlock ))
				{
					if (g_bDebug)
						std::cout << "ERROR: failed to process model.\n";

					return EXIT_FAILURE;
				}
			}
			else if (pTextLine->IsKey( "texture" ))
			{
				if (!UTIL_ProcessTexture( pResourceTextBlock ))
				{
					if (g_bDebug)
						std::cout << "ERROR: failed to process texture.\n";

					return EXIT_FAILURE;
				}
			}
		}

		delete[] sRootPath;
	}

	if (g_bDebug || g_bHelp)
		system( "pause" );

	return EXIT_SUCCESS;
}