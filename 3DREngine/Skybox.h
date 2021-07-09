#ifndef SKYBOX_H
#define SKYBOX_H

#include "Global.h"
#include "BaseDrawable.h"
#include "MaterialController.h"
#include "Model.h"

class CSkybox : public CBaseDrawable
{
public:
	DECLARE_CLASS( CSkybox, CBaseDrawable );

	CSkybox( const char *sMaterialPath, bool bShouldDraw, bool bActive );

	virtual void Draw( void );

	virtual bool ShouldDraw( void ) const;

private:
	CMaterialController *m_pMaterialController;
	CModel *m_pModel;
};

#endif // SKYBOX_H