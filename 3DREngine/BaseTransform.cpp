#include "BaseTransform.h"
#include "GlobalManager.h"

CBaseTransform::CBaseTransform()
{
	m_vec3Position = g_vec3Zero;
	m_qRotation = glm::quat( g_vec3Zero );
	m_vec3Scale = g_vec3One;

	m_pParent = NULL;

	m_bParentPosition = true;
	m_bParentRotation = true;
	m_bParentRotationPosition = true;
	m_bParentScale = true;
	m_bParentScalePosition = true;
	
	m_uiLastFramePositionUpdated = 0;
	m_uiLastFrameRotationUpdated = 0;
	m_uiLastFrameScaleUpdated = 0;
}


void CBaseTransform::Init( void )
{
	m_uiLastFramePositionUpdated = 0;
	m_uiLastFrameRotationUpdated = 0;
	m_uiLastFrameScaleUpdated = 0;

	BaseClass::Init();
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
	if (!m_pChildren.empty())
	{
		for (unsigned int i = 0; i < (unsigned int)m_pChildren.size(); i++)
		{
			if (m_pChildren[i]->ParentPosition())
				m_pChildren[i]->AddPosition( vec3Position );
		}
	}
	
	m_uiLastFramePositionUpdated = pGlobalValues->GetFrameCount();
}

void CBaseTransform::AddRotation( const glm::quat &qRotation )
{
	if (!m_pChildren.empty())
	{
		glm::quat qRotationInverse = glm::inverse( m_qRotation );

		m_qRotation = m_qRotation * qRotation;

		for (unsigned int i = 0; i < (unsigned int)m_pChildren.size(); i++)
		{
			if (m_pChildren[i]->ParentRotation())
			{
				m_pChildren[i]->SetRotation( m_qRotation * qRotationInverse * m_pChildren[i]->GetRotation() );
			}

			if (m_pChildren[i]->ParentRotationPosition())
			{
				glm::vec3 vec3Difference = m_pChildren[i]->GetPosition() - GetPosition();
				m_pChildren[i]->AddPosition( qRotationInverse * vec3Difference - vec3Difference );
				vec3Difference = m_pChildren[i]->GetPosition() - GetPosition();
				m_pChildren[i]->AddPosition( m_qRotation * vec3Difference - vec3Difference );
			}
		}
	}
	else
	{
		m_qRotation = m_qRotation * qRotation;
	}
	
	m_uiLastFrameRotationUpdated = pGlobalValues->GetFrameCount();
}

void CBaseTransform::AddScale( const glm::vec3 &vec3Scale )
{
	m_vec3Scale *= vec3Scale;
	if (!m_pChildren.empty())
	{
		for (unsigned int i = 0; i < (unsigned int)m_pChildren.size(); i++)
		{
			if (m_pChildren[i]->ParentScale())
				m_pChildren[i]->AddScale( vec3Scale );

			if (m_pChildren[i]->ParentScalePosition())
			{
				glm::vec3 vec3Difference = m_pChildren[i]->GetPosition() - GetPosition();
				m_pChildren[i]->AddPosition( m_vec3Scale * vec3Difference - vec3Difference );
			}
		}
	}

	m_uiLastFrameScaleUpdated = pGlobalValues->GetFrameCount();
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
	if (m_pParent)
		m_pParent->RemoveChild( this );

	if (pParent)
		pParent->AddChild( this );

	m_pParent = pParent;
}

void CBaseTransform::AddChild( CBaseTransform *pChild )
{
	m_pChildren.push_back( pChild );
}

void CBaseTransform::RemoveChild( CBaseTransform *pChild )
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

CBaseTransform *CBaseTransform::GetParent( void ) const
{
	return m_pParent;
}

const std::vector<CBaseTransform *> &CBaseTransform::GetChildren( void ) const
{
	return m_pChildren;
}

void CBaseTransform::SetParentPosition( bool bParentPosition )
{
	m_bParentPosition = bParentPosition;
}

void CBaseTransform::SetParentRotation( bool bParentRotation )
{
	m_bParentRotation = bParentRotation;
}

void CBaseTransform::SetParentRotationPosition( bool bParentRotationPosition )
{
	m_bParentRotationPosition = bParentRotationPosition;
}

void CBaseTransform::SetParentScale( bool bParentScale )
{
	m_bParentScale = bParentScale;
}

void CBaseTransform::SetParentScalePosition( bool bParentScalePosition )
{
	m_bParentScalePosition = bParentScalePosition;
}

bool CBaseTransform::ParentPosition( void ) const
{
	return m_bParentPosition;
}

bool CBaseTransform::ParentRotation( void ) const
{
	return m_bParentRotation;
}

bool CBaseTransform::ParentRotationPosition( void ) const
{
	return m_bParentRotationPosition;
}

bool CBaseTransform::ParentScale( void ) const
{
	return m_bParentScale;
}

bool CBaseTransform::ParentScalePosition( void ) const
{
	return m_bParentScalePosition;
}

bool CBaseTransform::PositionUpdated( void ) const
{
	return m_uiLastFramePositionUpdated == pGlobalValues->GetFrameCount();
}

bool CBaseTransform::RotationUpdated( void ) const
{
	return m_uiLastFrameRotationUpdated == pGlobalValues->GetFrameCount();
}

bool CBaseTransform::ScaleUpdated( void ) const
{
	return m_uiLastFrameScaleUpdated == pGlobalValues->GetFrameCount();
}