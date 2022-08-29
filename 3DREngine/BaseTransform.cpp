#include "BaseTransform.h"
#include "TimeManager.h"

DEFINE_DATADESC( CBaseTransform )

	DEFINE_FIELD( FL_MAP | FL_SAVED, glm::vec3, m_vec3Position, "position" )
	DEFINE_FIELD( FL_MAP | FL_SAVED, glm::quat, m_qRotation, "rotation" )
	DEFINE_FIELD( FL_MAP | FL_SAVED, glm::vec3, m_vec3Scale, "scale" )
	DEFINE_FIELD( FL_MAP | FL_SAVED, CHandle<CBaseTransform>, m_hParent, "parent" )
	DEFINE_FIELD( FL_MAP | FL_SAVED, std::vector<CHandle<CBaseTransform>>, m_hChildren, "children" )

END_DATADESC()

CBaseTransform::CBaseTransform()
{
	m_vec3Position = g_vec3Zero;
	m_qRotation = glm::quat( g_vec3Zero );
	m_vec3Scale = g_vec3One;

	m_hParent = NULL;

	AddFlags( FL_PARENTPOSITION | FL_PARENTROTATION | FL_PARENTSCALE );
	
	m_ulLastFramePositionUpdated = 0;
	m_ulLastFrameRotationUpdated = 0;
	m_ulLastFrameScaleUpdated = 0;
}

void CBaseTransform::SetPosition( const glm::vec3 &vec3Position )
{
	AddPosition( vec3Position - GetPosition() );
}

void CBaseTransform::SetRotation( const glm::quat &qRotation )
{
	AddRotation( glm::inverse( GetRotation() ) * qRotation );
}

void CBaseTransform::SetScale( const glm::vec3 &vec3Scale )
{
	AddScale( vec3Scale / GetScale() );
}

void CBaseTransform::AddPosition( const glm::vec3 &vec3Position )
{
	m_vec3Position += vec3Position;
	if (!m_hChildren.empty())
	{
		for (unsigned int i = 0; i < m_hChildren.size(); i++)
		{
			if (m_hChildren[i]->HasFlags( FL_PARENTPOSITION ))
				m_hChildren[i]->AddPosition( vec3Position );
		}
	}
	
	m_ulLastFramePositionUpdated = pTimeManager->GetFrameCount();
}

void CBaseTransform::AddRotation( const glm::quat &qRotation )
{
	if (!m_hChildren.empty())
	{
		glm::quat qRotationInverse = glm::inverse( m_qRotation );

		m_qRotation = m_qRotation * qRotation;

		for (unsigned int i = 0; i < m_hChildren.size(); i++)
		{
			if (m_hChildren[i]->HasFlags( FL_PARENTROTATION ))
			{
				m_hChildren[i]->SetRotation( m_qRotation * qRotationInverse * m_hChildren[i]->GetRotation() );
				glm::vec3 vec3Difference = m_hChildren[i]->GetPosition() - GetPosition();
				m_hChildren[i]->AddPosition( qRotationInverse * vec3Difference - vec3Difference );
				vec3Difference = m_hChildren[i]->GetPosition() - GetPosition();
				m_hChildren[i]->AddPosition( m_qRotation * vec3Difference - vec3Difference );
			}
		}
	}
	else
	{
		m_qRotation = m_qRotation * qRotation;
	}
	
	m_ulLastFrameRotationUpdated = pTimeManager->GetFrameCount();
}

void CBaseTransform::AddScale( const glm::vec3 &vec3Scale )
{
	m_vec3Scale *= vec3Scale;
	if (!m_hChildren.empty())
	{
		for (unsigned int i = 0; i < m_hChildren.size(); i++)
		{
			if (m_hChildren[i]->HasFlags( FL_PARENTSCALE ))
			{
				m_hChildren[i]->AddScale( vec3Scale );
				glm::vec3 vec3Difference = m_hChildren[i]->GetPosition() - GetPosition();
				m_hChildren[i]->AddPosition( m_vec3Scale * vec3Difference - vec3Difference );
			}
		}
	}

	m_ulLastFrameScaleUpdated = pTimeManager->GetFrameCount();
}

const glm::vec3 &CBaseTransform::GetPosition( void ) const
{
	return m_vec3Position;
}

const glm::quat &CBaseTransform::GetRotation( void ) const
{
	return m_qRotation;
}

const glm::vec3 &CBaseTransform::GetScale( void ) const
{
	return m_vec3Scale;
}

void CBaseTransform::SetParent( CBaseTransform *pParent )
{
	if (m_hParent)
		m_hParent->RemoveChild( this );

	if (pParent)
		pParent->AddChild( this );

	m_hParent = pParent;
}

void CBaseTransform::AddChild( CBaseTransform *pChild )
{
	m_hChildren.push_back( pChild );
}

void CBaseTransform::RemoveChild( CBaseTransform *pChild )
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

bool CBaseTransform::PositionUpdated( void ) const
{
	return m_ulLastFramePositionUpdated == pTimeManager->GetFrameCount();
}

bool CBaseTransform::RotationUpdated( void ) const
{
	return m_ulLastFrameRotationUpdated == pTimeManager->GetFrameCount();
}

bool CBaseTransform::ScaleUpdated( void ) const
{
	return m_ulLastFrameScaleUpdated == pTimeManager->GetFrameCount();
}