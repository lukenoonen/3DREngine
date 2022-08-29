#ifndef BASEHUDELEMENT_H
#define BASEHUDELEMENT_H

#include "Global.h"
#include "BaseHUDTransform.h"

class CBaseHUDElement : public CBaseHUDTransform
{
public:
	DECLARE_CLASS( CBaseHUDElement, CBaseHUDTransform );

	CBaseHUDElement( const glm::vec2 &vecPosition, const glm::vec2 &vecScale, float flPriority, bool bShouldDraw, bool bActive );

	virtual void Draw( void );

private:
	unsigned int m_uiVAO;
	unsigned int m_uiVBO;
	unsigned int m_uiEBO;
};

#endif // BASEHUDELEMENT_H