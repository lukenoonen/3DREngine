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

	if (m_hParent && m_hParent->AddChild( this ))
	{
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

	std::list<CHandle<CBaseTransform>>::iterator it = m_hChildren.begin();
	while (it != m_hChildren.end())
	{
		if (!it->Check())
		{
			it = m_hChildren.erase( it );
			continue;
		}
		it++;
	}

	BaseClass::PostThink();
}

void CBaseTransform::CleanUp( void )
{
	m_hParent.Check();
	m_hChildren.clear();

	BaseClass::CleanUp();
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
		std::list<CHandle<CBaseTransform>>::iterator it;
		for (it = m_hChildren.begin(); it != m_hChildren.end(); it++)
		{
			CBaseTransform *pChild = *it;
			if (pChild->HasFlags( fl_parentposition.GetFlag() ))
				pChild->AddParentPosition( vec3Position );
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

		std::list<CHandle<CBaseTransform>>::iterator it;
		for (it = m_hChildren.begin(); it != m_hChildren.end(); it++)
		{
			CBaseTransform *pChild = *it;
			if (pChild->HasFlags( fl_parentrotation.GetFlag() ))
				pChild->AddParentRotation( GetPosition(), GetRotation(), qRotationInverse, qRotationDelta );
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
		std::list<CHandle<CBaseTransform>>::iterator it;
		for (it = m_hChildren.begin(); it != m_hChildren.end(); it++)
		{
			CBaseTransform *pChild = *it;
			if (pChild->HasFlags( fl_parentscale.GetFlag() ))
				pChild->AddParentScale( vec3Scale, GetPosition(), GetScale() );
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

bool CBaseTransform::AddChild( CBaseTransform *pChild )
{
	if (std::find( m_hChildren.begin(), m_hChildren.end(), pChild ) == m_hChildren.end())
	{
		m_hChildren.emplace_back( pChild );
		return true;
	}

	return false;
}

bool CBaseTransform::RemoveChild( CBaseTransform *pChild )
{
	std::list<CHandle<CBaseTransform>>::iterator it = std::find( m_hChildren.begin(), m_hChildren.end(), pChild );
	if (it != m_hChildren.end())
	{
		m_hChildren.erase( it );
		return true;
	}

	return false;
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