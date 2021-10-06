#ifndef MODELNODE_H
#define MODELNODE_H

#include "Global.h"

class CModelNode
{
public:
	DECLARE_CLASS_NOBASE( CModelNode );

	CModelNode( bool bAssociatedWithBone, unsigned int uiAssociatedBone, const glm::mat4 &matTransformation, const std::vector<unsigned int> &uiChildren );

	bool IsAssociatedWithBone( void );
	unsigned int GetAssociatedBone( void );

	const glm::mat4 &GetTransformation( void );

	unsigned int GetChildrenCount( void );
	unsigned int GetChildren( unsigned int uiIndex );

private:
	bool m_bAssociatedWithBone;
	unsigned int m_uiAssociatedBone;
	glm::mat4 m_matTransformation;
	std::vector<unsigned int> m_uiChildren;
};

#endif // MODELNODE_H