#ifndef BASEMATERIAL_H
#define BASEMATERIAL_H

#include "Global.h"
#include "BaseAsset.h"
#include "RenderManager.h"

class CBaseMaterial : public CBaseAsset
{
public:
	DECLARE_CLASS( CBaseMaterial, CBaseAsset )

	CBaseMaterial();

	virtual EShaderType GetShaderType( void );

	virtual bool ShouldApply( void );
	virtual void Apply( void );
};

#endif // BASEMATERIAL_H