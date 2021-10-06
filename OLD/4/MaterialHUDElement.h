#ifndef MATERIALHUDELEMENT_H
#define MATERIALHUDELEMENT_H

#include "Global.h"
#include "BaseHUDElement.h"
#include "MaterialController.h"

class CMaterialHUDElement : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CMaterialHUDElement, CBaseHUDElement );

	CMaterialHUDElement( const char *sMaterialPath, const glm::vec2 &vecPosition, const glm::vec2 &vecScale, float flPriority, bool bShouldDraw, bool bActive );

	virtual void Draw( void );

	virtual bool ShouldDraw( void ) const;

private:
	CMaterialController *m_pMaterialController;
};

#endif // MATERIALHUDELEMENT_H