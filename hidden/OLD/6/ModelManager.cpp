#include "ModelManager.h"
#include <UTIL.h>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GlobalManager.h"
#include "Mesh.h"
#include "ModelNode.h"

CModelManager::CModelManager()
{
	for (unsigned int i = 0; i < GENERICMODELS_COUNT; i++)
		m_pGenericModels[i] = CreateModel( g_sGenericModelPaths[i] );
}

CModelManager::~CModelManager()
{
	Reset();

	for (unsigned int i = 0; i < GENERICMODELS_COUNT; i++)
		delete m_pGenericModels[i];
}

void CModelManager::Reset( void )
{
	for (unsigned int i = 0; i < m_pModels.size(); i++)
		delete m_pModels[i];

	m_pModels.clear();
}

CModel *CModelManager::GetModel( const char *sPath )
{
	for (unsigned int i = 0; i < GENERICMODELS_COUNT; i++)
	{
		if (UTIL_streq( g_sGenericModelPaths[i], sPath ))
			return m_pGenericModels[i];
	}

	for (unsigned int i = 0; i < m_pModels.size(); i++)
	{
		if (UTIL_streq( m_pModels[i]->GetPath(), sPath ))
			return m_pModels[i];
	}

	CModel *pNewModel = CreateModel( sPath );
	m_pModels.push_back( pNewModel );
	return pNewModel;
}

CModel *CModelManager::GetGenericModel( GenericModels_t tGenericModel )
{
	return m_pGenericModels[tGenericModel];
}

CModel *CModelManager::CreateModel( const char *sPath )
{
	char sFullPath[260];
	sprintf_s( sFullPath, sizeof(sFullPath), "resources/models/%s", sPath );

	CModel *pNewModel = NULL;

	std::fstream fModel( sFullPath, std::ios::in | std::ios::binary );
	if (!fModel.is_open())
		ExitError( "Cannot open model file." );

	std::vector<CMesh *> pMeshes;
	std::vector<CModelNode *> pModelNodes;
	std::vector<glm::mat4> matBoneOffsets;

	char cType;
	while (UTIL_Read( fModel, &cType, 1, char ))
	{
		if (cType == 'b')
		{
			glm::mat4 matOffset;
			UTIL_Read( fModel, &matOffset, 1, glm::mat4 );

			matBoneOffsets.push_back( matOffset );
		}
		else if (cType == 'm')
		{
			std::vector<SVertex> verVertices;
			std::vector<unsigned int> uiIndices;

			unsigned int uiNumVerticies;
			UTIL_Read( fModel, &uiNumVerticies, 1, unsigned int );
			for (unsigned int i = 0; i < uiNumVerticies; i++)
			{
				SVertex verNewVertex;
				UTIL_Read( fModel, &verNewVertex.vecPosition, 1, glm::vec3 );
				UTIL_Read( fModel, &verNewVertex.vecNormal, 1, glm::vec3 );
				UTIL_Read( fModel, &verNewVertex.vecTangent, 1, glm::vec3 );
				UTIL_Read( fModel, &verNewVertex.vecBitangent, 1, glm::vec3 );
				UTIL_Read( fModel, &verNewVertex.vecTexCoords, 1, glm::vec2 );

				unsigned int uiNumAffectingBones;
				UTIL_Read( fModel, &uiNumAffectingBones, 1, unsigned int );

				for (unsigned int j = 0; j < uiNumAffectingBones; j++)
				{
					unsigned int uiBoneInfoIndex;
					UTIL_Read( fModel, &uiBoneInfoIndex, 1, unsigned int );
					
					float flWeight;
					UTIL_Read( fModel, &flWeight, 1, float );

					verNewVertex.vecBoneIDs[j] = uiBoneInfoIndex;
					verNewVertex.vecWeights[j] = flWeight;
				}
				
				verVertices.push_back( verNewVertex );
			}

			unsigned int uiNumFaces;
			UTIL_Read( fModel, &uiNumFaces, 1, unsigned int );
			for (unsigned int i = 0; i < uiNumFaces; i++)
			{
				unsigned int uiNumIndices;
				UTIL_Read( fModel, &uiNumIndices, 1, unsigned int );
				for (unsigned int j = 0; j < uiNumIndices; j++)
				{
					unsigned int uiIndex;
					UTIL_Read( fModel, &uiIndex, 1, unsigned int );
					uiIndices.push_back( uiIndex );
				}
			}

			pMeshes.push_back( new CMesh( verVertices, uiIndices ) );
		}
		else if (cType == 'n')
		{
			unsigned int uiIndex;
			bool bAssociatedWithBone;
			unsigned int uiAssociatedBone = 0;
			glm::mat4 matTransformation;
			std::vector<unsigned int> uiMeshes;
			std::vector<unsigned int> uiChildren;

			UTIL_Read( fModel, &uiIndex, 1, unsigned int );
			if (uiIndex >= pModelNodes.size())
				pModelNodes.resize( uiIndex + 1 );

			UTIL_Read( fModel, &bAssociatedWithBone, 1, bool );
			if (bAssociatedWithBone)
				UTIL_Read( fModel, &uiAssociatedBone, 1, unsigned int );

			UTIL_Read( fModel, &matTransformation, 1, glm::mat4 );

			unsigned int uiNumMeshes;
			UTIL_Read( fModel, &uiNumMeshes, 1, unsigned int );
			for (unsigned int i = 0; i < uiNumMeshes; i++)
			{
				unsigned int uiMesh;
				UTIL_Read( fModel, &uiMesh, 1, unsigned int );
				uiMeshes.push_back( uiMesh );
			}

			unsigned int uiNumChildren;
			UTIL_Read( fModel, &uiNumChildren, 1, unsigned int );

			for (unsigned int i = 0; i < uiNumChildren; i++)
			{
				unsigned int uiChild;
				UTIL_Read( fModel, &uiChild, 1, unsigned int );
				uiChildren.push_back( uiChild );
			}

			pModelNodes[uiIndex] = new CModelNode( bAssociatedWithBone, uiAssociatedBone, matTransformation, uiMeshes, uiChildren );
		}
	}

	return new CModel( sPath, pMeshes, pModelNodes, matBoneOffsets );
}