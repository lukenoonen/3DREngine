#ifndef MODEL_H
#define MODEL_H

#include "Global.h"
#include "Shader.h"
#include "Mesh.h"
#include "ModelNode.h"
#include "Animation.h"

class CModel
{
public:
	DECLARE_CLASS_NOBASE( CModel );

	CModel( const char *sPath, const std::vector<CMesh *> &pMeshes, const std::vector<CModelNode *> &pModelNodes, const std::vector<glm::mat4> &matBoneOffsets );
	~CModel();

	void Draw( CMaterialController *pMaterialController );

	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransform, CAnimation *pAnimation, float flAnimationTime );

	const char *GetPath( void ) const;
	unsigned int GetBoneTransformCount( void ) const;
	unsigned int GetMeshCount( void ) const;
    
private:
	void ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransform, CAnimation *pAnimation, float flAnimationTime, unsigned int uiNodeIndex, const glm::mat4 &matParentTransform );

private:
	char *m_sPath;

	std::vector<CMesh *> m_pMeshes;
	std::vector<CModelNode *> m_pModelNodes;

	std::vector<glm::mat4> m_matBoneOffsets;
};

#endif // MODEL_H