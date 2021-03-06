#ifndef OrthoWORLDCAMERA_H
#define OrthoWORLDCAMERA_H

#include "Global.h"
#include "BaseWorldCamera.h"

class COrthoWorldCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( COrthoWorldCamera, CBaseWorldCamera );

	COrthoWorldCamera();

	virtual void Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

	virtual int BindTexture( void );

	void SetHeight( float flHeight );

	void SetNear( float flNear );

	void SetFar( float flFar );

protected:
	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

private:
	float m_flHeight;
	float m_flNear;
	float m_flFar;

	glm::mat4 m_matProjection;
	glm::mat4 m_matView;
	glm::mat4 m_matTotal;
	glm::mat4 m_matTotalLocked;

	bool m_bUpdateProjection;

	unsigned int m_uiTextureFBO;
	unsigned int m_uiTexture;
};

#endif // OrthoWORLDCAMERA_H