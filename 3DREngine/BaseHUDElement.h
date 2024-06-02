#ifndef BASEHUDELEMENT_H
#define BASEHUDELEMENT_H

#include "Global.h"
#include "BaseWorld2D.h"
#include "BasePlayerCamera.h"

glm::vec3 UTIL_LocalToCamera( const glm::vec3 &vec3Local, const glm::mat4 &matTotal );
glm::vec3 UTIL_LocalToCamera( const glm::vec2 &vec2Local, const glm::mat4 &matTotal );
glm::vec3 UTIL_CameraToLocal( const glm::vec3 &vec3Camera, const glm::mat4 &matTotalInverse );
glm::vec3 UTIL_CameraToLocal( const glm::vec2 &vec2Camera, const glm::mat4 &matTotalInverse );

enum class EMouseOverType : EBaseEnum
{
	t_focus,
	t_hover,

	i_count,
	i_invalid = i_count,
};

class CBaseHUDElement : public CBaseWorld2D
{
public:
	DECLARE_CLASS( CBaseHUDElement, CBaseWorld2D )

	DECLARE_LINKED_CLASS()

	CBaseHUDElement();

	virtual void CleanUp( void );

	virtual void PostThink( void );

	virtual void SetHUDCamera( CBasePlayerCamera *pHUDCamera );

	virtual CBaseHUDElement *GetMouseOver( EMouseOverType eMouseOverType );

	void Click( void );
	virtual void OnClick( void );
	virtual void PropagateClick( void );
	void Release( void );
	virtual void OnRelease( void );
	virtual void PropagateRelease( void );
	bool IsClicked( void ) const;

	void GainFocus( void );
	virtual void OnGainFocus( void );
	virtual void PropagateGainFocus( void );
	void LoseFocus( void );
	virtual void OnLoseFocus( void );
	virtual void PropagateLoseFocus( void );
	bool HasFocus( void ) const;

	void Hover( void );
	virtual void OnHover( void );
	virtual void PropagateHover( void );
	void Unhover( void );
	virtual void OnUnhover( void );
	virtual void PropagateUnhover( void );
	bool HasHover( void ) const;

	virtual void ClearFocus( void );
	virtual void ClearHover( void );

	virtual bool FocusCheck( void );
	virtual bool HoverCheck( void );

protected:
	CBasePlayerCamera *GetHUDCamera( void ) const;

	float GetDepth( void ) const;

private:
	void UpdateBounds( void );

	bool IsMouseOver( void ) const;

private:
	CBasePlayerCamera *m_pHUDCamera;
	float m_flDepth;
	glm::vec2 m_vec2Bounds[4];
	glm::vec2 m_vec2Min;
	glm::vec2 m_vec2Max;

	bool m_bClicked;
	bool m_bFocus;
	bool m_bHover;
};

#endif // BASEHUDELEMENT_H