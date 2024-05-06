#ifndef BASEHUDELEMENT_H
#define BASEHUDELEMENT_H

#include "Global.h"
#include "BaseWorld2D.h"
#include "BasePlayerCamera.h"

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

	virtual void PreRender( void );

	void SetHUDCamera( CBasePlayerCamera *pHUDCamera );

	virtual bool GetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *&pMouseOver );

	void Click( void );
	virtual void OnClick( void );
	void Release( void );
	virtual void OnRelease( void );
	bool IsClicked( void ) const;

	void GainFocus( void );
	virtual void OnGainFocus( void );
	void LoseFocus( void );
	virtual void OnLoseFocus( void );
	bool HasFocus( void ) const;

	void Hover( void );
	virtual void OnHover( void );
	void Unhover( void );
	virtual void OnUnhover( void );
	bool HasHover( void ) const;

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