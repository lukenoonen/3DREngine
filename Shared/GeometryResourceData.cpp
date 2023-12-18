#include "GeometryResourceData.h"

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, SGeometryResourceData &dData )
{
	unsigned int uiVertexCount;
	if (!pFile->Read( uiVertexCount ))
		return false;

	dData.verVertices.resize( uiVertexCount );

	for (unsigned int i = 0; i < uiVertexCount; i++)
	{
		SVertex &verVertex = dData.verVertices[i];
		if (!pFile->Read( verVertex.vec3Position ) || !pFile->Read( verVertex.vec3Normal ) || !pFile->Read( verVertex.vec3Tangent ) || !pFile->Read( verVertex.vec3Bitangent ) || !pFile->Read( verVertex.vec2TexCoords ))
			return false;



		unsigned int uiNumAffectingBones;
		if (!pFile->Read( uiNumAffectingBones ))
			return false;

		for (unsigned int j = 0; j < uiNumAffectingBones; j++)
		{
			if (!pFile->Read( verVertex.vec4BoneIDs[j] ) || !pFile->Read( verVertex.vec4Weights[j] ))
				return false;
		}
	}

	unsigned int uiIndicesCount;
	if (!pFile->Read( uiIndicesCount ))
		return false;

	dData.uiIndices.resize( uiIndicesCount );

	for (unsigned int i = 0; i < uiIndicesCount; i++)
	{
		if (!pFile->Read( dData.uiIndices[i] ))
			return false;
	}

	return true;
}
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_Write( CFile *pFile, SGeometryResourceData &dData )
{
	unsigned int uiVertexCount = (unsigned int)dData.verVertices.size();
	if (!pFile->Write( uiVertexCount ))
		return false;

	for (unsigned int i = 0; i < uiVertexCount; i++)
	{
		SVertex &verVertex = dData.verVertices[i];
		if (!pFile->Write( verVertex.vec3Position ) || !pFile->Write( verVertex.vec3Normal ) || !pFile->Write( verVertex.vec3Tangent ) || !pFile->Write( verVertex.vec3Bitangent ) || !pFile->Write( verVertex.vec2TexCoords ))
			return false;

		unsigned int uiNumAffectingBones;
		for (uiNumAffectingBones = 0; uiNumAffectingBones < 4; uiNumAffectingBones++)
		{
			if (verVertex.vec4Weights[uiNumAffectingBones] == 0.0f)
				break;
		}

		if (!pFile->Write( uiNumAffectingBones ))
			return false;

		for (unsigned int j = 0; j < uiNumAffectingBones; j++)
		{
			if (!pFile->Write( verVertex.vec4BoneIDs[j] ) || !pFile->Write( verVertex.vec4Weights[j] ))
				return false;
		}
	}

	unsigned int uiIndicesCount = (unsigned int)dData.uiIndices.size();
	if (!pFile->Write( uiIndicesCount ))
		return false;

	for (unsigned int i = 0; i < uiIndicesCount; i++)
	{
		if (!pFile->Write( dData.uiIndices[i] ))
			return false;
	}

	return true;
}
#endif // PROJECT_RESOURCE