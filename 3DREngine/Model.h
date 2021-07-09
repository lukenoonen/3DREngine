#ifndef MODEL_H
#define MODEL_H

#include "Global.h"
#include "Shader.h"
#include "Mesh.h"
#include "ModelNode.h"
#include "Animation.h"

#include "MaterialController.h"
#include "AnimationController.h"

class CModel
{
public:
	DECLARE_CLASS_NOBASE( CModel );

	CModel( const char *sPath, const std::vector<CMesh *> &pMeshes, const std::vector<CModelNode *> &pModelNodes, const std::vector<glm::mat4> &matBoneOffsets );
	~CModel();

	void Draw( CMaterialController *pMaterialController );

	void UpdateAnimation( CAnimationController *pAnimationController );

	const char *GetPath( void );

	unsigned int GetBoneTransformSize( void );
    
private:

	void ReadNodeHierarchy( CAnimationController *pAnimationController, CAnimation *pAnimation, float flAnimationTime, unsigned int uiNodeIndex, const glm::mat4 &matParentTransform );

	char *m_sPath;

	std::vector<CMesh *> m_pMeshes;
	std::vector<CModelNode *> m_pModelNodes;

	std::vector<glm::mat4> m_matBoneOffsets;

	glm::mat4 m_matGlobalInverseTransform;
};

#endif // MODEL_H