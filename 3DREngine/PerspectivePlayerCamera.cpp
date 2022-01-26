#include "PerspectivePlayerCamera.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "EntityManager.h"

CPerspectivePlayerCamera::CPerspectivePlayerCamera()
{

}

void CPerspectivePlayerCamera::Init( void )
{
	const glm::ivec2 &vecSize = GetSize();
	m_matProjection = glm::perspective( glm::radians( cf_r_fov.GetValue() ), (float)vecSize.x / (float)vecSize.y, cf_r_near.GetValue(), cf_r_far.GetValue() );
	m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
	m_matTotal = m_matProjection * m_matView;
	m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );

	BaseClass::Init();
}

void CPerspectivePlayerCamera::PostThink( void )
{
	bool bUpdateTotal = false;

	if (cf_r_fov.WasDispatched() || cf_r_near.WasDispatched() || cf_r_far.WasDispatched())
	{
		const glm::ivec2 &vecSize = GetSize();
		m_matProjection = glm::perspective( glm::radians( cf_r_fov.GetValue() ), (float)vecSize.x / (float)vecSize.y, cf_r_near.GetValue(), cf_r_far.GetValue() );
		bUpdateTotal = true;
	}

	if (PositionUpdated() || RotationUpdated())
	{
		m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
		bUpdateTotal = true;
	}

	if (bUpdateTotal)
	{
		m_matTotal = m_matProjection * m_matView;
		m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );
	}

	BaseClass::PostThink();
}

void CPerspectivePlayerCamera::Render( void )
{
	const glm::ivec2 &vecSize = GetSize();
	bool bMSAA = GetMSAALevel() != 0;

	pRenderManager->SetViewportSize( vecSize );
	pRenderManager->SetFrameBuffer( bMSAA ? GetMSAAFBO() : 0 );

	pShaderManager->SetUniformBufferObject( UBO_VIEW, 0, &m_matTotal );
	pShaderManager->SetUniformBufferObject( UBO_VIEW, 1, &m_matTotalLocked );
	pShaderManager->SetUniformBufferObject( UBO_VIEW, 2, &GetPosition() );

	pEntityManager->DrawEntities();

	if (bMSAA)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, GetMSAAFBO() );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glBlitFramebuffer( 0, 0, vecSize.x, vecSize.y, 0, 0, vecSize.x, vecSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	}
}