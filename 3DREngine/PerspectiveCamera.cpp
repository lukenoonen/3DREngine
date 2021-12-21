#include "PerspectiveCamera.h"
/*
CPerspectiveCamera::CPerspectiveCamera( float flFOV, float flNear, float flFar, bool bDepth, unsigned int uiTextureCount, unsigned int uiMSAALevel, const glm::ivec2 &vecSize, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( bDepth, uiTextureCount, uiMSAALevel, vecSize, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flFOV = flFOV;
	m_flNear = flNear;
	m_flFar = flFar;

	SetMatricies( glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp ), glm::perspective( m_flFOV, (float)vecSize.x / (float)vecSize.y, m_flNear, m_flFar ) );
}

void CPerspectiveCamera::PostThnk( void )
{
	BaseClass::PostThink();

	if (ShouldUpdateProjection())
	{
		const glm::ivec2 &vecSize = GetSize();
		SetProjection( glm::perspective( m_flFOV, (float)vecSize.x / (float)vecSize.y, m_flNear, m_flFar ) );
		ResetUpdateProjection();
	}

	if (HasTransformUpdated())
	{
		SetView( glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp ) );
		ResetTransformUpdated();
	}
}

void CPerspectiveCamera::SetFOV( float flFOV )
{
	if (m_flFOV != flFOV)
	{
		m_flFOV = flFOV;
		MarkUpdateProjection();
	}
}

void CPerspectiveCamera::SetNear( float flNear )
{
	if (m_flNear != flNear)
	{
		m_flNear = flNear;
		MarkUpdateProjection();
	}
}

void CPerspectiveCamera::SetFar( float flFar )
{
	if (m_flFar != flFar)
	{
		m_flFar = flFar;
		MarkUpdateProjection();
	}
}

float CPerspectiveCamera::GetFOV( void ) const
{
	return m_flFOV;
}

float CPerspectiveCamera::GetNear( void ) const
{
	return m_flFar;
}

float CPerspectiveCamera::GetFar( void ) const
{
	return m_flFar;
}*/