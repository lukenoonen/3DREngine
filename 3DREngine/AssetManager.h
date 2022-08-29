#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Global.h"
#include "TextReader.h"

// TODO: figure out where these header structs and defines should be located
struct SAnimationChannel
{
	std::vector<float> flPositionTimes;
	std::vector<glm::vec3> vec3Positions;

	std::vector<float> flRotationTimes;
	std::vector<glm::quat> qRotations;

	std::vector<float> flScaleTimes;
	std::vector<glm::vec3> vec3Scales;
};

struct SVertex
{
	glm::vec3 vec3Position;
	glm::vec3 vec3Normal;
	glm::vec3 vec3Tangent;
	glm::vec3 vec3Bitangent;
	glm::vec2 vec2TexCoords;
	glm::ivec4 vec4BoneIDs;
	glm::vec4 vec4Weights;
};

struct SSkeletonBone
{
	unsigned short usIndex;
	glm::mat4 matOffset;
};

struct SSkeletonNode
{
	~SSkeletonNode()
	{
		delete pSkeletonBone;
		for (unsigned int i = 0; i < pChildren.size(); i++)
			delete pChildren[i];
	}

	unsigned short usIndex;
	SSkeletonBone *pSkeletonBone;
	std::vector<SSkeletonNode *> pChildren;
};

class CAssetManager // TODO: maybe implement some caching so that files aren't loaded more than once in most cases, if this then change image to texture
{
public:
	DECLARE_CLASS_NOBASE( CAssetManager )

	CAssetManager();

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
};

#endif // ASSETMANAGER_H