#include "BaseHUDTransform.h"

CBaseHUDTransform::CBaseHUDTransform( const glm::vec2 &vecPosition, const glm::vec2 &vecScale, float flPriority, bool bShouldDraw, bool bActive ) : BaseClass( bShouldDraw, bActive )
{
	m_vecPosition = vecPosition;
	m_vecScale = vecScale;
	m_flPriority = flPriority;

	m_bParentPosition = true;
	m_bParentScale = true;
	m_bParentScalePosition = true;
	m_bParentPriority = true;
}

void CBaseHUDTransform::SetPosition( const glm::vec2 &vecPosition )
{
	AddPosition( vecPosition - GetPosition() );
}

void CBaseHUDTransform::SetScale( const glm::vec2 &vecScale )
{
	AddScale( vecScale / GetScale() );
}

void CBaseHUDTransform::SetPriority( float flPriority )
{
	AddPriority( flPriority - m_flPriority );
}

void CBaseHUDTransform::AddPosition( const glm::vec2 &vecPosition )
{
	m_vecPosition += vecPosition;
	if (!m_pChildren.empty())
	{
		for (unsigned int i = 0; i < m_pChildren.size(); i++)
		{
			if (m_pChildren[i]->ParentPosition())
				m_pChildren[i]->AddPosition( vecPosition );
		}
	}
}

void CBaseHUDTransform::AddScale( const glm::vec2 &vecScale )
{
	m_vecScale *= vecScale;
	if (!m_pChildren.empty())
	{
		for (unsigned int i = 0; i < m_pChildren.size(); i++)
		{
			if (m_pChildren[i]->ParentScale())
				m_pChildren[i]->AddScale( vecScale );

			if (m_pChildren[i]->ParentScalePosition())
			{
				glm::vec2 vecDifference = m_pChildren[i]->GetPosition() - GetPosition();
				m_pChildren[i]->AddPosition( m_vecScale * vecDifference - vecDifference );
			}
		}
	}
}

void CBaseHUDTransform::AddPriority( float flPriority )
{
	m_flPriority += flPriority;
	if (!m_pChildren.empty())
	{
		for (unsigned int i = 0; i < m_pChildren.size(); i++)
		{
			if (m_pChildren[i]->ParentPriority())
				m_pChildren[i]->AddPriority( flPriority );
		}
	}
}

const glm::vec2 &CBaseHUDTransform::GetPosition( void ) const
{
	return m_vecPosition;
}

const glm::vec2 &CBaseHUDTransform::GetScale( void ) const
{
	return m_vecScale;
}

float CBaseHUDTransform::GetPriority( void ) const
{
	return m_flPriority;
}

void CBaseHUDTransform::SetParent( CBaseHUDTransform *pParent )
{
	if (m_pParent)
		m_pParent->RemoveChild( this );

	if (pParent)
	{
		m_pParent = pParent;
		pParent->AddChild( this );
	}
	else
	{
		m_pParent = NULL;
	}
}

void CBaseHUDTransform::AddChild( CBaseHUDTransform *pChild )
{
	m_pChildren.push_back( pChild );
}

void CBaseHUDTransform::RemoveChild( CBaseHUDTransform *pChild )
{
	for (unsigned int i = 0; i < m_pChildren.size(); i++)
	{
		if (m_pChildren[i] == pChild)
		{
			m_pChildren.erase( m_pChildren.begin() + i );
			break;
		}
	}
}

void CBaseHUDTransform::SetParentPosition( bool bParentPosition )
{
	m_bParentPosition = bParentPosition;
}

void CBaseHUDTransform::SetParentScale( bool bParentScale )
{
	m_bParentScale = bParentScale;
}

void CBaseHUDTransform::SetParentScalePosition( bool bParentScalePosition )
{
	m_bParentScalePosition = bParentScalePosition;
}

void CBaseHUDTransform::SetParentPriority( bool bParentPriority )
{
	m_bParentPriority = bParentPriority;
}

bool CBaseHUDTransform::ParentPosition( void ) const
{
	return m_bParentPosition;
}

bool CBaseHUDTransform::ParentScale( void ) const
{
	return m_bParentScale;
}

bool CBaseHUDTransform::ParentScalePosition( void ) const
{
	return m_bParentScalePosition;
}

bool CBaseHUDTransform::ParentPriority( void ) const
{
	return m_bParentPriority;
}