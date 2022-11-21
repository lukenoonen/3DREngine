#include "OrthoPlayerCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

DEFINE_LINKED_CLASS( COrthoPlayerCamera, camera_player_ortho )

COrthoPlayerCamera::COrthoPlayerCamera()
{

}

bool COrthoPlayerCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	const glm::ivec2 &vec2Size = GetSize();
	float flHeight = cf_r_height.GetValue() * 0.5f;
	float flWidth = flHeight * (float)vec2Size.x / (float)vec2Size.y;
	m_matProjection = glm::ortho( -flWidth, flWidth, -flHeight, flHeight, cf_r_near.GetValue(), cf_r_far.GetValue() );
	m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vec3Front, GetRotation() * g_vec3Up );
	m_matTotal = m_matProjection * m_matView;
	m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );

	return true;
}

void COrthoPlayerCamera::PostThink( void )
{
	bool bUpdateTotal = false;

	if (cf_r_height.WasDispatched() || cf_r_near.WasDispatched() || cf_r_far.WasDispatched())
	{
		const glm::ivec2 &vec2Size = GetSize();
		float flHeight = cf_r_height.GetValue() * 0.5f;
		float flWidth = flHeight * (float)vec2Size.x / (float)vec2Size.y;
		m_matProjection = glm::ortho( -flWidth, flWidth, -flHeight, flHeight, cf_r_near.GetValue(), cf_r_far.GetValue() );
		bUpdateTotal = true;
	}

	if (PositionUpdated() || RotationUpdated())
	{
		m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vec3Front, GetRotation() * g_vec3Up );
		bUpdateTotal = true;
	}

	if (bUpdateTotal)
	{
		m_matTotal = m_matProjection * m_matView;
		m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );
	}

	BaseClass::PostThink();
}

void COrthoPlayerCamera::Render( void )
{
	const glm::ivec2 &vec2Size = GetSize();
	bool bMSAA = GetMSAALevel() != 0;

	pRenderManager->SetViewportSize( vec2Size );
	pRenderManager->SetFrameBuffer( bMSAA ? GetMSAAFBO() : 0 );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 0, &m_matTotal );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 1, &m_matTotalLocked );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 2, &GetPosition() );

	pEntityManager->DrawEntities();

	if (bMSAA)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, GetMSAAFBO() );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glBlitFramebuffer( 0, 0, vec2Size.x, vec2Size.y, 0, 0, vec2Size.x, vec2Size.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	}
}