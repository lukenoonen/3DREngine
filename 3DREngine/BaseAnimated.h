#ifndef BASEANIMATED_H
#define BASEANIMATED_H

#include "Global.h"
#include "BaseModeled.h"
#include "AnimationController.h"

class CBaseAnimated : public CBaseModeled
{
public:
	DECLARE_CLASS( CBaseAnimated, CBaseModeled );

	CBaseAnimated( const char *sAnimationControllerPath, const char *sMaterialPath, const char *sModelPath, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void PostThink( void );
	virtual void PreDraw( void );

private:
	CAnimationController *m_pAnimationController;
};

#endif // BASEANIMATED_H