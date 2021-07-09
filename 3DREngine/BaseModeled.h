#ifndef BASEMODELED_H
#define BASEMODELED_H

#include "Global.h"
#include "BaseWorld.h"
#include "Model.h"
#include "MaterialController.h"

class CBaseModeled : public CBaseWorld
{
public:
	DECLARE_CLASS( CBaseModeled, CBaseWorld );

	CBaseModeled( const char *sMaterialPath, const char *sModelPath, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void PreDraw( void );
	virtual void Draw( void );

	CModel *GetModel( void );

	virtual bool ShouldDraw( void ) const;

private:
	CModel *m_pModel;
	CMaterialController *m_pMaterialController;
};

#endif // BASEMODELED_H