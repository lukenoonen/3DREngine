#include "BaseTransform.h"
#include "TimeManager.h"

DEFINE_DATADESC( CBaseTransform )

	DEFINE_FIELD( DataField, CMonitoredValue<glm::vec3>, m_vec3Position, "position", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<glm::quat>, m_qRotation, "rotation", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<glm::vec3>, m_vec3Scale, "scale", FL_NONE )
	DEFINE_FIELD( LinkedDataField, CHandle<CBaseTransform>, m_hParent, "parent", FL_NONE )

END_DATADESC()

CBaseTransform::CBaseTransform()
{
	AddFlags( fl_parentposition.GetFlag() | fl_parentrotation.GetFlag() | fl_parentscale.GetFlag() );

	m_vec3Position = g_vec3Zero;
	m_qRotation = glm::quat(g_vec3Zero);
	m_vec3Scale = g_vec3One;
}

bool CBaseTransform::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (m_hParent)
	{
		m_hParent->AddChild( this );
		if (HasFlags(fl_parentposition.GetFlag()))
			AddParentPosition( m_hParent->GetPosition() );
		if (HasFlags( fl_parentrotation.GetFlag() ))
			AddParentRotation( m_hParent->GetPosition(), m_hParent->GetRotation(), g_qZero, m_hParent->GetRotation() );
		if (HasFlags( fl_parentscale.GetFlag() ))
			AddParentScale( m_hParent->GetScale(), m_hParent->GetPosition(), m_hParent->GetScale() );
	}

	return true;
}

void CBaseTransform::PostThink( void )
{
	m_hParent.Check();

	for (unsigned int i = 0; i < m_hChildren.size(); i++)
	{
		if (!m_hChildren[i].Check())
			m_hChildren.erase( m_hChildren.begin() + i-- ); // TODO: verify that every time something is erased like this it accounts for the deleted element
	}

	BaseClass::PostThink();
}

void CBaseTransform::OnRemove( void )
{
	m_hParent.Check();

	for (unsigned int i = 0; i < m_hChildren.size(); i++)
		m_hChildren[i].Check();

	BaseClass::OnRemove();
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
	m_vec3Position = GetPosition() + vec3Position;
	if (!m_hChildren.empty())
	{
		for (unsigned int i = 0; i < m_hChildren.size(); i++)
		{
			if (m_hChildren[i]->HasFlags( fl_parentposition.GetFlag() ))
				m_hChildren[i]->AddParentPosition( vec3Position );
		}
	}
}

void CBaseTransform::AddRotation( const glm::quat &qRotation )
{
	if (!m_hChildren.empty())
	{
		glm::quat qRotationInverse = glm::inverse( GetRotation() );

		m_qRotation = GetRotation() * qRotation;

		glm::quat qRotationDelta = GetRotation() * qRotationInverse;

		for (unsigned int i = 0; i < m_hChildren.size(); i++)
		{
			if (m_hChildren[i]->HasFlags( fl_parentrotation.GetFlag() ))
				m_hChildren[i]->AddParentRotation( GetPosition(), GetRotation(), qRotationInverse, qRotationDelta );
		}
	}
	else
	{
		m_qRotation = GetRotation() * qRotation;
	}
}

void CBaseTransform::AddScale( const glm::vec3 &vec3Scale )
{
	m_vec3Scale = GetScale() * vec3Scale;
	if (!m_hChildren.empty())
	{
		for (unsigned int i = 0; i < m_hChildren.size(); i++)
		{
			if (m_hChildren[i]->HasFlags( fl_parentscale.GetFlag() ))
				m_hChildren[i]->AddParentScale( vec3Scale, GetPosition(), GetScale() );
		}
	}
}

const glm::vec3 &CBaseTransform::GetPosition( void ) const
{
	return m_vec3Position.Get();
}

const glm::quat &CBaseTransform::GetRotation( void ) const
{
	return m_qRotation.Get();
}

const glm::vec3 &CBaseTransform::GetScale( void ) const
{
	return m_vec3Scale.Get();
}

void CBaseTransform::SetParent( CBaseTransform *pParent )
{
	if (m_hParent)
		m_hParent->RemoveChild( this );

	if (pParent)
		pParent->AddChild( this );

	m_hParent = pParent;
}

void CBaseTransform::SetParentRelative( CBaseTransform *pParent )
{
	if (m_hParent)
		m_hParent->RemoveChild( this );

	if (pParent)
	{
		pParent->AddChild( this );
		if (HasFlags( fl_parentposition.GetFlag() ))
			AddParentPosition( pParent->GetPosition() );
		if (HasFlags( fl_parentrotation.GetFlag() ))
			AddParentRotation( pParent->GetPosition(), pParent->GetRotation(), g_qZero, pParent->GetRotation() );
		if (HasFlags( fl_parentscale.GetFlag() ))
			AddParentScale( pParent->GetScale(), pParent->GetPosition(), pParent->GetScale() );
	}

	m_hParent = pParent;
}

void CBaseTransform::AddChild( CBaseTransform *pChild )
{
	m_hChildren.emplace_back( pChild );
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
	return m_vec3Position.Modified();
}

bool CBaseTransform::RotationUpdated( void ) const
{
	return m_qRotation.Modified();
}

bool CBaseTransform::ScaleUpdated( void ) const
{
	return m_vec3Scale.Modified();
}

void CBaseTransform::AddParentPosition( const glm::vec3 &vec3Position )
{
	AddPosition( vec3Position );
}

void CBaseTransform::AddParentRotation( const glm::vec3 &vec3ParentPosition, const glm::quat &qParentRotation, const glm::quat &qRotationInverse, const glm::quat &qRotationDelta )
{
	SetRotation( qRotationDelta * GetRotation() );
	glm::vec3 vec3Difference = GetPosition() - vec3ParentPosition;
	AddPosition( qRotationInverse * vec3Difference - vec3Difference );
	vec3Difference = GetPosition() - vec3ParentPosition;
	AddPosition( qParentRotation * vec3Difference - vec3Difference );
}

void CBaseTransform::AddParentScale( const glm::vec3 &vec3Scale, const glm::vec3 &vec3ParentPosition, const glm::vec3 &vec3ParentScale )
{
	AddScale( vec3Scale );
	glm::vec3 vec3Difference = GetPosition() - vec3ParentPosition;
	AddPosition( vec3ParentScale * vec3Difference - vec3Difference );
}