#include "ModelNode.h"

CModelNode::CModelNode( bool bAssociatedWithBone, unsigned int uiAssociatedBone, const glm::mat4 &matTransformation, const std::vector<unsigned int> &uiChildren )
{
	m_bAssociatedWithBone = bAssociatedWithBone;
	m_uiAssociatedBone = uiAssociatedBone;
	m_matTransformation = matTransformation;
	m_uiChildren = uiChildren;
}

bool CModelNode::IsAssociatedWithBone( void )
{
	return m_bAssociatedWithBone;
}

unsigned int CModelNode::GetAssociatedBone( void )
{
	return m_uiAssociatedBone;
}

const glm::mat4 &CModelNode::GetTransformation( void )
{
	return m_matTransformation;
}

unsigned int CModelNode::GetChildrenCount( void )
{
	return (unsigned int)m_uiChildren.size();
}

unsigned int CModelNode::GetChildren( unsigned int uiIndex )
{
	return m_uiChildren[uiIndex];
}