#include "PortalCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

DEFINE_LINKED_CLASS( CPortalCamera, camera_portal )

DEFINE_DATADESC( CPortalCamera )

	DEFINE_FIELD( DataField, glm::vec3, m_vec3DisplayPosition, "displayposition", FL_REQUIRED )
	DEFINE_FIELD( DataField, glm::quat, m_qDisplayRotation, "displayrotation", FL_REQUIRED )

END_DATADESC()

CPortalCamera::CPortalCamera()
{

}

bool CPortalCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	glm::vec3 vec3Normal = GetRotation() * g_vec3Front;
	m_vec4Plane = glm::vec4( vec3Normal, -glm::dot( vec3Normal, GetPosition() + vec3Normal * 0.0001f ) );
	m_matReflection = glm::translate( g_matIdentity, m_vec3DisplayPosition - g_vec3Front * 40.0f + g_vec3Up * 10.0f ); // glm::inverse(glm::translate( g_matIdentity, m_vec3DisplayPosition ) * glm::toMat4( m_qDisplayRotation )) * (glm::translate( g_matIdentity, GetPosition() ) * glm::toMat4( GetRotation() ));

	CBaseCamera *pTargetCamera = GetTargetCamera();

	const glm::vec3 &vec3TargetPosition = pTargetCamera->GetPosition();
	const glm::quat &qTargetRotation = pTargetCamera->GetRotation();

	const glm::ivec2 &vec2Size = GetSize();
	m_matProjection = glm::perspective( glm::radians( cf_r_fov.GetValue() ), (float)vec2Size.x / (float)vec2Size.y, cf_r_near.GetValue(), cf_r_far.GetValue() );
	m_matView = glm::lookAt( glm::vec3( m_matReflection * glm::vec4( vec3TargetPosition, 1.0f ) ), glm::vec3( m_matReflection * glm::vec4( (vec3TargetPosition + qTargetRotation * g_vec3Front), 1.0f ) ), glm::vec3( m_matReflection * glm::vec4( qTargetRotation * g_vec3Up, 1.0f ) ) );
	m_matTotal = m_matProjection * m_matView;
	m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );

	return true;
}

#include <iostream>

void CPortalCamera::PostThink( void )
{
	CBaseCamera *pTargetCamera = GetTargetCamera();

	bool bUpdateTotal = false;
	bool bUpdateView = pTargetCamera->PositionUpdated() || pTargetCamera->RotationUpdated();

	if (cf_r_fov.WasDispatched() || cf_r_near.WasDispatched() || cf_r_far.WasDispatched())
	{
		const glm::ivec2 &vec2Size = GetSize();
		m_matProjection = glm::perspective( glm::radians( cf_r_fov.GetValue() ), (float)vec2Size.x / (float)vec2Size.y, cf_r_near.GetValue(), cf_r_far.GetValue() );
		bUpdateTotal = true;
	}

	if (PositionUpdated() || RotationUpdated())
	{
		glm::vec3 vec3Normal = GetRotation() * g_vec3Front;
		m_vec4Plane = glm::vec4( vec3Normal, -glm::dot( vec3Normal, GetPosition() ) );
		m_matReflection = glm::translate( g_matIdentity, glm::vec3(0.0f, 1.0f, 0.0f) );  // m_matReflection = glm::inverse( glm::translate( g_matIdentity, m_vec3DisplayPosition ) * glm::toMat4( m_qDisplayRotation ) ) * glm::translate( g_matIdentity, GetPosition() ) * glm::toMat4( GetRotation() );

		bUpdateView = true;
	}

	if (bUpdateView)
	{
		const glm::vec3 &vec3TargetPosition = pTargetCamera->GetPosition();
		const glm::quat &qTargetRotation = pTargetCamera->GetRotation();

		glm::mat4 matTransform = glm::inverse( glm::translate( g_matIdentity, m_vec3DisplayPosition - vec3TargetPosition ) );

		glm::vec3 vec3Position = glm::vec3( matTransform * glm::vec4( GetPosition(), 1.0f ));
		glm::vec3 vec3Target = glm::vec3( matTransform * glm::vec4( (GetPosition() + qTargetRotation * g_vec3Front), 1.0f));
		glm::vec3 vec3Up = glm::vec3( matTransform * glm::vec4( GetPosition() + qTargetRotation * g_vec3Up, 1.0f ) ) - vec3Position;

		m_matView = glm::lookAt( vec3Position, vec3Target, vec3Up );

		std::cout << vec3Position.x << ' ' << vec3Position.y << ' ' << vec3Position.z << " vs " << vec3TargetPosition.x << ' ' << vec3TargetPosition.y << ' ' << vec3TargetPosition.z << '\n';

		bUpdateTotal = true;
	}

	if (bUpdateTotal)
	{
		m_matTotal = m_matProjection * m_matView;
		m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );
	}

	BaseClass::PostThink();
}

void CPortalCamera::Render( void )
{
	pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_clip, (EBaseEnum)EShaderPreprocessorClip::t_true );
	glEnable( GL_CLIP_DISTANCE0 );

	pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_reflection, (EBaseEnum)EShaderPreprocessorReflection::t_false );

	pEntityManager->SetTextureCamera( this );

	const glm::ivec2 &vec2Size = GetSize();
	bool bMSAA = GetMSAALevel() != 0;

	pRenderManager->SetViewportSize( vec2Size );
	pRenderManager->SetFrameBuffer( bMSAA ? GetMSAAFBO() : m_glTextureFBO );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 0, &m_matTotal );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 1, &m_matTotalLocked );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 2, &GetPosition() );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_clip, 0, &m_vec4Plane );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_reflection, 0, &m_matTotal );

	pEntityManager->DrawEntities();

	if (bMSAA)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, GetMSAAFBO() );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_glTextureFBO );
		glBlitFramebuffer( 0, 0, vec2Size.x, vec2Size.y, 0, 0, vec2Size.x, vec2Size.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	}

	pEntityManager->SetTextureCamera( this );

	pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_clip, (EBaseEnum)EShaderPreprocessorClip::t_false );
	glDisable( GL_CLIP_DISTANCE0 );

	pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_reflection, (EBaseEnum)EShaderPreprocessorReflection::t_true );
}

int CPortalCamera::BindTexture( void )
{
	return pRenderManager->BindTexture( m_glTexture, GL_TEXTURE_2D );
}

void CPortalCamera::CreateTextureBuffers( void )
{
	const glm::ivec2 &vec2Size = GetSize();

	glGenFramebuffers( 1, &m_glTextureFBO );
	glGenTextures( 1, &m_glTexture );

	glBindTexture( GL_TEXTURE_2D, m_glTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, vec2Size.x, vec2Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
	glBindFramebuffer( GL_FRAMEBUFFER, m_glTextureFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_glTexture, 0 );
}

void CPortalCamera::DestroyTextureBuffers( void )
{
	pRenderManager->UnbindTexture( m_glTexture );

	glDeleteFramebuffers( 1, &m_glTextureFBO );
	glDeleteTextures( 1, &m_glTexture );
}