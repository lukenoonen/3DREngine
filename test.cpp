#include <iostream>
#include <fstream>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include "SharedGlobal.h"
#include "TextReader.h"
#include "SharedAsset.h"

bool g_bDebug = false;
bool g_bHelp = false;
const char *g_sFilePath = NULL;

static inline glm::mat4 mat4_cast( const aiMatrix4x4 &m ) { return glm::transpose( glm::make_mat4( &m.a1 ) ); }

bool CreateModel( const char *sRootPath, const char *sName, CTextInformation *pTextInformation )
{
	const char *sModel;
	if (!pTextInformation->GetString( "model", sModel ))
	{
		if (g_bDebug)
			std::cout << "ERROR: model not found.\n";

		return false;
	}

	bool bFlipUV;
	if (!pTextInformation->GetBool( "flipuv", bFlipUV ))
	{
		if (g_bDebug)
			std::cout << "ERROR: flipuv not found.\n";

		return false;
	}

	char *sModelPath = UTIL_stradd( sRootPath, sModel );
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
	std::unordered_map<std::string, unsigned int> g_uiBoneMapping;

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
	{
		aiMesh *pMesh = pScene->mMeshes[i];

		if (pMesh->HasBones())
		{
			for (unsigned int i = 0; i < pMesh->mNumBones; i++)
			{
				std::string sBoneName( pMesh->mBones[i]->mName.data );
				if (g_uiBoneMapping.find( sBoneName ) == g_uiBoneMapping.end())
				{
					g_uiBoneMapping[sBoneName] = (unsigned int)matBoneOffsets.size();
					matBoneOffsets.push_back( mat4_cast( pMesh->mBones[i]->mOffsetMatrix ) );
				}
			}
		}

		char *sFullGeometryName;
		if (!pMesh->mName.C_Str() || !*pMesh->mName.C_Str())
		{
			if (pScene->mNumMeshes == 1)
			{
				sFullGeometryName = UTIL_stradd( sName, ".3gm" );
			}
			else
			{
				char *sIndex = UTIL_uitoa( i );
				char *sGeometryName = UTIL_stradd( sName, sIndex );
				sFullGeometryName = UTIL_stradd( sGeometryName, ".3gm" );
				delete[] sGeometryName;
				delete[] sIndex;
			}
		}
		else
		{
			sFullGeometryName = UTIL_stradd( pMesh->mName.C_Str(), ".3gm" );
		}

		char *sGeometryOutputPath = UTIL_stradd( sRootPath, sFullGeometryName );
		std::fstream fGeometryOutput( sGeometryOutputPath, std::ios::out | std::ios::binary );
		delete[] sGeometryOutputPath;
		delete[] sFullGeometryName;

		UTIL_Write( fGeometryOutput, &pMesh->mNumVertices, 1, unsigned int );
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			UTIL_Write( fGeometryOutput, &pMesh->mVertices[i].x, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mVertices[i].y, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mVertices[i].z, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mNormals[i].x, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mNormals[i].y, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mNormals[i].z, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mTangents[i].x, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mTangents[i].y, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mTangents[i].z, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mBitangents[i].x, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mBitangents[i].y, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mBitangents[i].z, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mTextureCoords[0][i].x, 1, float );
			UTIL_Write( fGeometryOutput, &pMesh->mTextureCoords[0][i].y, 1, float );

			if (pMesh->HasBones())
			{
				std::vector<unsigned int> uiBoneIDs;
				std::vector<float> flBoneWeights;

				for (unsigned int j = 0; j < pMesh->mNumBones; j++)
				{
					unsigned int uiNumWeights = pMesh->mBones[j]->mNumWeights;
					aiVertexWeight *pWeights = pMesh->mBones[j]->mWeights;
					for (unsigned int k = 0; k < uiNumWeights; k++)
					{
						if (pWeights[k].mVertexId == i)
						{
							std::string sBoneName( pMesh->mBones[j]->mName.data );
							uiBoneIDs.push_back( g_uiBoneMapping[sBoneName] );
							flBoneWeights.push_back( pWeights[k].mWeight );
							break;
						}
					}

					if ((unsigned int)uiBoneIDs.size() >= 4)
						break;
				}

				unsigned int iNumAffectingBones = (unsigned int)uiBoneIDs.size();
				UTIL_Write( fGeometryOutput, &iNumAffectingBones, 1, unsigned int );
				for (unsigned int j = 0; j < iNumAffectingBones; j++)
				{
					UTIL_Write( fGeometryOutput, &uiBoneIDs[j], 1, unsigned int );
					UTIL_Write( fGeometryOutput, &flBoneWeights[j], 1, float );
				}
			}
			else
			{
				unsigned int uiTemp = 0;
				UTIL_Write( fGeometryOutput, &uiTemp, 1, unsigned int );
			}
		}

		UTIL_Write( fGeometryOutput, &pMesh->mNumFaces, 1, unsigned int );
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const aiFace &face = pMesh->mFaces[i];
			UTIL_Write( fGeometryOutput, &face.mNumIndices, 1, unsigned int );
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				UTIL_Write( fGeometryOutput, &face.mIndices[j], 1, unsigned int );
		}

		fGeometryOutput.close();
	}

	char *sFullSkeletonName = UTIL_stradd( sName, ".3sk" );
	char *sSkeletonOutputPath = UTIL_stradd( sRootPath, sFullSkeletonName );
	std::fstream fSkeletonOutput( sSkeletonOutputPath, std::ios::out | std::ios::binary );
	delete[] sSkeletonOutputPath;
	delete[] sFullSkeletonName;

	unsigned int uiSkeletonBoneCount = (unsigned int)matBoneOffsets.size();
	UTIL_Write( fSkeletonOutput, &uiSkeletonBoneCount, 1, unsigned int );
	
	for (unsigned int i = 0; i < uiSkeletonBoneCount; i++)
		UTIL_Write( fSkeletonOutput, &matBoneOffsets[i], 1, glm::mat4 );
	
	std::vector<aiNode *> pNodes;
	std::unordered_map<std::string, unsigned int> g_uiNodeMapping;
	pNodes.push_back( pScene->mRootNode );
	for (unsigned int i = 0; i < pNodes.size(); i++)
	{
		aiNode *pNode = pNodes[i];

		std::string sNodeName = pNode->mName.C_Str();
		if (!sNodeName.empty())
			g_uiNodeMapping[sNodeName] = i;

		for (unsigned int j = 0; j < pNode->mNumChildren; j++)
			pNodes.push_back( pNode->mChildren[j] );
	}

	unsigned int uiSkeletonNodeCount = (unsigned int)pNodes.size();
	UTIL_Write( fSkeletonOutput, &uiSkeletonNodeCount, 1, unsigned int );

	for (unsigned int i = 0; i < uiSkeletonNodeCount; i++)
	{
		aiNode *pNode = pNodes[i];

		std::string sNodeName = pNode->mName.C_Str();
		bool bHasSkeletonBone = g_uiBoneMapping.find( sNodeName ) != g_uiBoneMapping.end();
		UTIL_Write( fSkeletonOutput, &bHasSkeletonBone, 1, bool );

		if (bHasSkeletonBone)
			UTIL_Write( fSkeletonOutput, &g_uiBoneMapping[sNodeName], 1, unsigned int );
	}

	unsigned int uiChildrenOffset = 1;
	for (unsigned int i = 0; i < uiSkeletonNodeCount; i++)
	{
		aiNode *pNode = pNodes[i];

		UTIL_Write( fSkeletonOutput, &pNode->mNumChildren, 1, unsigned int );
		for (unsigned int i = 0; i < pNode->mNumChildren; i++)
		{
			unsigned int uiWriteIndex = uiChildrenOffset + i;
			UTIL_Write( fSkeletonOutput, &uiWriteIndex, 1, unsigned int );
		}

		uiChildrenOffset += pNode->mNumChildren;
	}

	fSkeletonOutput.close();

	for (unsigned int i = 0; i < pScene->mNumAnimations; i++)
	{
		aiAnimation *pAnimation = pScene->mAnimations[i];
		char *sFullAnimationName;
		if (!pAnimation->mName.C_Str() || !*pAnimation->mName.C_Str())
		{
			if (pScene->mNumAnimations == 1)
			{
				sFullAnimationName = UTIL_stradd( sName, ".3an" );
			}
			else
			{
				char *sIndex = UTIL_uitoa( i );
				char *sAnimationName = UTIL_stradd( sName, sIndex );
				sFullAnimationName = UTIL_stradd( sAnimationName, ".3an" );
				delete[] sAnimationName;
				delete[] sIndex;
			}
		}
		else
		{
			sFullAnimationName = UTIL_stradd( pAnimation->mName.C_Str(), ".3an" );
		}

		char *sAnimationOutputPath = UTIL_stradd( sRootPath, sFullAnimationName );
		std::fstream fAnimationOutput( sAnimationOutputPath, std::ios::out | std::ios::binary );
		delete[] sAnimationOutputPath;
		delete[] sFullAnimationName;

		float flTime = (float)pAnimation->mDuration / (float)pAnimation->mTicksPerSecond;
		UTIL_Write( fAnimationOutput, &flTime, 1, float );

		UTIL_Write( fAnimationOutput, &uiSkeletonNodeCount, 1, unsigned int );

		UTIL_Write( fAnimationOutput, &pAnimation->mNumChannels, 1, unsigned int );

		for (unsigned int i = 0; i < pAnimation->mNumChannels; i++)
		{
			aiNodeAnim *pNodeAnim = pAnimation->mChannels[i];
			std::string sNodeName = pNodeAnim->mNodeName.C_Str();
			if (g_uiNodeMapping.find( sNodeName ) == g_uiNodeMapping.end())
			{
				if (g_bDebug)
					std::cout << "WARNING: Cannot find associated node '" << sNodeName << "' in animation node!\n";

				continue;
			}

			UTIL_Write( fAnimationOutput, &g_uiNodeMapping[sNodeName], 1, unsigned int );

			UTIL_Write( fAnimationOutput, &pNodeAnim->mNumPositionKeys, 1, unsigned int );
			for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys; i++)
			{
				float flWriteTime = (float)pNodeAnim->mPositionKeys[i].mTime / (float)pAnimation->mTicksPerSecond;
				UTIL_Write( fAnimationOutput, &flWriteTime, 1, float );
				UTIL_Write( fAnimationOutput, &pNodeAnim->mPositionKeys[i].mValue.x, 1, float );
				UTIL_Write( fAnimationOutput, &pNodeAnim->mPositionKeys[i].mValue.y, 1, float );
				UTIL_Write( fAnimationOutput, &pNodeAnim->mPositionKeys[i].mValue.z, 1, float );
			}

			UTIL_Write( fAnimationOutput, &pNodeAnim->mNumRotationKeys, 1, unsigned int );
			for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys; i++)
			{
				float flWriteTime = (float)pNodeAnim->mRotationKeys[i].mTime / (float)pAnimation->mTicksPerSecond;
				UTIL_Write( fAnimationOutput, &flWriteTime, 1, float );
				UTIL_Write( fAnimationOutput, &pNodeAnim->mRotationKeys[i].mValue.w, 1, float );
				UTIL_Write( fAnimationOutput, &pNodeAnim->mRotationKeys[i].mValue.x, 1, float );
				UTIL_Write( fAnimationOutput, &pNodeAnim->mRotationKeys[i].mValue.y, 1, float );
				UTIL_Write( fAnimationOutput, &pNodeAnim->mRotationKeys[i].mValue.z, 1, float );
			}

			UTIL_Write( fAnimationOutput, &pNodeAnim->mNumScalingKeys, 1, unsigned int );
			for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys; i++)
			{
				float flWriteTime = (float)pNodeAnim->mScalingKeys[i].mTime / (float)pAnimation->mTicksPerSecond;
				UTIL_Write( fAnimationOutput, &flWriteTime, 1, float );
				UTIL_Write( fAnimationOutput, &pNodeAnim->mScalingKeys[i].mValue.x, 1, float );
				UTIL_Write( fAnimationOutput, &pNodeAnim->mScalingKeys[i].mValue.y, 1, float );
				UTIL_Write( fAnimationOutput, &pNodeAnim->mScalingKeys[i].mValue.z, 1, float );
			}
		}

		fAnimationOutput.close();
	}

	return true;
}

bool CreateImage( const char *sRootPath, const char *sName, CTextInformation *pTextInformation )
{
	const char *sImage;
	if (!pTextInformation->GetString( "image", sImage ))
	{
		if (g_bDebug)
			std::cout << "ERROR: image not found.\n";

		return false;
	}

	char *sFullImagePath = UTIL_stradd( sRootPath, sImage );
	int iWidth, iHeight, iChannels;
	unsigned char *pData = stbi_load( sFullImagePath, &iWidth, &iHeight, &iChannels, 0 );
	delete[] sFullImagePath;

	char *sFullTextureName = UTIL_stradd( sName, ".3im" );
	char *sTextureOutputPath = UTIL_stradd( sRootPath, sFullTextureName );
	std::fstream fTextureOutput( sTextureOutputPath, std::ios::out | std::ios::binary );
	delete[] sTextureOutputPath;
	delete[] sFullTextureName;

	UTIL_Write( fTextureOutput, &iWidth, 1, int );
	UTIL_Write( fTextureOutput, &iHeight, 1, int );
	UTIL_Write( fTextureOutput, &iChannels, 1, int );
	UTIL_Write( fTextureOutput, pData, iWidth * iHeight * iChannels, unsigned char );

	stbi_image_free( pData );

	return true;
}

int main( int iArgC, const char *sArgV[] )
{
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

	if (g_bHelp) // FIXME: innacurate
		std::cout << "INFO: 3DRAsset is a tool used for compiling assets for 3DREngine-based programs.\nTo use this program, run it with a source file as an argument. If the source file is of a proper format, this program will output the associated compiled file(s) with the proper file extension(s).\nParameters for this program are:\n-debug, which provides a detailed program output;\n-help, which display this information;\n-nomaterial, which prevents the compilation of materials;\n-nomaterialcontroller, which prevents the compilation of material controllers;\nFor information on how to format source files, please follow the instructions located in the reademe.\n";

	if (g_sFilePath)
	{
		char *sText = UTIL_readf( g_sFilePath );

		if (!sText)
		{
			if (g_bDebug)
				std::cout << "ERROR: file information could not be read.\n";

			return 0;
		}

		CTextReader trTextReader( sText );
		CTextInformation *pTextInformation = trTextReader.GetTextInformation();

		if (!pTextInformation->IsSuccess())
		{
			if (g_bDebug)
				std::cout << "ERROR: file information could not be successfully interpreted.\n";

			return 0;
		}

		const char *sName;
		if (!pTextInformation->GetString( "name", sName ))
		{
			if (g_bDebug)
				std::cout << "ERROR: name could not be found.\n";

			return 0;
		}

		char *sRootPath = "";
		const char *sChar = UTIL_strchrl( g_sFilePath, "/\\" );
		if (sChar)
			sRootPath = UTIL_stredit( g_sFilePath, (unsigned int)(sChar - g_sFilePath + 1) );

		const char *sType;
		if (!pTextInformation->GetString( "type", sType ))
		{
			if (g_bDebug)
				std::cout << "ERROR: type could not be found.\n";

			return 0;
		}

		else if (UTIL_streq( sType, "model" ))
		{
			CreateModel( sRootPath, sName, pTextInformation );
		}
		else if (UTIL_streq( sType, "image" ))
		{
			CreateImage( sRootPath, sName, pTextInformation );
		}
		else
		{
			if (g_bDebug)
				std::cout << "ERROR: type could not be identified.\n";

			return 0;
		}

		if (sRootPath != "")
			delete[] sRootPath;
	}

	if (g_bDebug || g_bHelp)
		system( "pause" );

	return 0;
}