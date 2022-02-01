#include "PlanarReflectionCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "AssetManager.h"

CPlanarReflectionCamera::CPlanarReflectionCamera()
{

}

void CPlanarReflectionCamera::Init( void )
{
	BaseClass::Init();

	glm::vec3 vecDirection = GetRotation() * g_vec3Front;
	m_vec4Plane.x = vecDirection.x;
	m_vec4Plane.y = vecDirection.y;
	m_vec4Plane.z = vecDirection.z;
	m_vec4Plane.w = glm::dot( vecDirection, GetPosition() );

	const glm::ivec2 &vec2Size = GetSize();
	m_matProjection = glm::perspective( glm::radians( cf_r_fov.GetValue() ), (float)vec2Size.x / (float)vec2Size.y, cf_r_near.GetValue(), cf_r_far.GetValue() );
	m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vec3Front, GetRotation() * g_vec3Up );
	m_matTotal = m_matProjection * m_matView;
	m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );
}

void CPlanarReflectionCamera::PostThink( void )
{
	bool bUpdateTotal = false;

	if (cf_r_fov.WasDispatched() || cf_r_near.WasDispatched() || cf_r_far.WasDispatched())
	{
		const glm::ivec2 &vec2Size = GetSize();
		m_matProjection = glm::perspective( glm::radians( cf_r_fov.GetValue() ), (float)vec2Size.x / (float)vec2Size.y, cf_r_near.GetValue(), cf_r_far.GetValue() );
		bUpdateTotal = true;
	}

	if (PositionUpdated() || RotationUpdated())
	{
		glm::vec3 vecDirection = GetRotation() * g_vec3Front;
		m_vec4Plane.x = vecDirection.x;
		m_vec4Plane.y = vecDirection.y;
		m_vec4Plane.z = vecDirection.z;
		m_vec4Plane.w = glm::dot( vecDirection, GetPosition() );

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

void CPlanarReflectionCamera::Render( void )
{
	pEntityManager->SetTextureCamera( this );

	const glm::ivec2 &vec2Size = GetSize();
	bool bMSAA = GetMSAALevel() != 0;

	pRenderManager->SetViewportSize( vec2Size );
	pRenderManager->SetFrameBuffer( bMSAA ? GetMSAAFBO() : m_uiTextureFBO );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 0, &m_matTotal );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 1, &m_matTotalLocked );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 2, &GetPosition() );

	pEntityManager->DrawEntities();

	if (bMSAA)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, GetMSAAFBO() );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_uiTextureFBO );
		glBlitFramebuffer( 0, 0, vec2Size.x, vec2Size.y, 0, 0, vec2Size.x, vec2Size.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	}

	pEntityManager->SetTextureCamera( this );
}

int CPlanarReflectionCamera::BindTexture( void )
{
	return pAssetManager->BindTexture( m_uiTexture, GL_TEXTURE_2D );
}

void CPlanarReflectionCamera::CreateTextureBuffers( void )
{
	const glm::ivec2 &vec2Size = GetSize();

	glGenFramebuffers( 1, &m_uiTextureFBO );
	glGenTextures( 1, &m_uiTexture );

	glBindTexture( GL_TEXTURE_2D, m_uiTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, vec2Size.x, vec2Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glBindFramebuffer( GL_FRAMEBUFFER, m_uiTextureFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTexture, 0 );
}

void CPlanarReflectionCamera::DestroyTextureBuffers( void )
{
	pAssetManager->UnbindTexture( m_uiTexture );

	glDeleteFramebuffers( 1, &m_uiTextureFBO );
	glDeleteTextures( 1, &m_uiTexture );
}