#include "BaseTransform2D.h"

DEFINE_DATADESC( CBaseTransform2D )

	DEFINE_FIELD( DataField, glm::vec2, m_vec2Position, "position", 0 )
	DEFINE_FIELD( DataField, float, m_flRotation, "rotation", 0 )
	DEFINE_FIELD( DataField, glm::vec2, m_vec2Scale, "scale", 0 )
	DEFINE_FIELD( LinkedDataField, CHandle<CBaseTransform2D>, m_hParent, "parent", 0 )
	DEFINE_FIELD( LinkedVectorDataField, CHandle<CBaseTransform2D>, m_hChildren, "children", 0 )

END_DATADESC()

CBaseTransform2D::CBaseTransform2D()
{
	m_vec2Position = g_vec2Zero;
	m_flRotation = 0.0f;
	m_vec2Scale = g_vec2One;
}

void CBaseTransform2D::PostThink( void )
{
	m_hParent.Check();

	for (unsigned int i = 0; i < m_hChildren.size(); i++)
	{
		if (!m_hChildren[i].Check())
			m_hChildren.erase( m_hChildren.begin() + i-- ); // TODO: verify that every time something is erased like this it accounts for the deleted element
	}

	BaseClass::PostThink();
}

void CBaseTransform2D::SetPosition( const glm::vec2 &vec2Position )
{
	AddPosition( vec2Position - GetPosition() );
}

void CBaseTransform2D::SetRotation( float flRotation )
{
	AddRotation( flRotation - m_flRotation );
}

void CBaseTransform2D::SetScale( const glm::vec2 &vec2Scale )
{
	AddScale( vec2Scale / GetScale() );
}

void CBaseTransform2D::AddPosition( const glm::vec2 &vec2Position )
{
	m_vec2Position += vec2Position;
	if (!m_hChildren.empty())
	{
		for (unsigned int i = 0; i < m_hChildren.size(); i++)
		{
			if (m_hChildren[i]->HasFlags( fl_parentposition.GetFlag() ))
				m_hChildren[i]->AddPosition( vec2Position );
		}
	}

	//MarkPositionUpdated();
}

void CBaseTransform2D::AddRotation( float flRotation )
{
	m_flRotation += flRotation;
	if (!m_hChildren.empty())
	{
		for (unsigned int i = 0; i < m_hChildren.size(); i++)
		{
			if (m_hChildren[i]->HasFlags( fl_parentrotation.GetFlag() ))
			{
				m_hChildren[i]->AddRotation( flRotation );
				glm::vec2 vec2Difference = m_hChildren[i]->GetPosition() - GetPosition();
				float flSin = glm::sin( m_flRotation );
				float flCos = glm::cos( m_flRotation );
				glm::vec2 vec2NewDifference = glm::vec2( vec2Difference.x * flCos - vec2Difference.y * flSin, vec2Difference.x * flSin + vec2Difference.y * flCos );
				m_hChildren[i]->AddPosition( vec2NewDifference - vec2Difference );
			}
		}
	}

	//MarkRotationUpdated();
}

void CBaseTransform2D::AddScale( const glm::vec2 &vec2Scale )
{
	m_vec2Scale *= vec2Scale;
	if (!m_hChildren.empty())
	{
		for (unsigned int i = 0; i < m_hChildren.size(); i++)
		{
			if (m_hChildren[i]->HasFlags( fl_parentscale.GetFlag() ))
			{
				m_hChildren[i]->AddScale( vec2Scale );
				glm::vec2 vec2Difference = m_hChildren[i]->GetPosition() - GetPosition();
				m_hChildren[i]->AddPosition( m_vec2Scale * vec2Difference - vec2Difference );
			}
		}
	}

	//MarkScaleUpdated();
}

const glm::vec2 &CBaseTransform2D::GetPosition( void ) const
{
	return m_vec2Position;
}

float CBaseTransform2D::GetRotation( void ) const
{
	return m_flRotation;
}

const glm::vec2 &CBaseTransform2D::GetScale( void ) const
{
	return m_vec2Scale;
}

void CBaseTransform2D::SetParent( CBaseTransform2D *pParent )
{
	if (m_hParent)
		m_hParent->RemoveChild( this );

	if (pParent)
		pParent->AddChild( this );

	m_hParent = pParent;
}

void CBaseTransform2D::AddChild( CBaseTransform2D *pChild )
{
	m_hChildren.push_back( pChild );
}

void CBaseTransform2D::RemoveChild( CBaseTransform2D *pChild )
{
	for (unsigned int i = 0; i < m_hChildren.size(); i++)
	{
		if (m_hChildren[i] == pChild)
		{
			m_hChildren.erase( m_hChildren.begin() + i );
			break;
		}
	}
}