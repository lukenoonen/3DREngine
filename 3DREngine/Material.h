#ifndef MATERIAL_H
#define MATERIAL_H

#include "Global.h"
#include "BaseAsset.h"
#include "RenderManager.h"
#include "ShaderManager.h"

class CMaterial : public CBaseAsset
{
public:
	DECLARE_CLASS( CMaterial, CBaseAsset );

	CMaterial( const char *sPath );

	virtual bool ShouldDraw( void ) const;

	virtual void Use( void );

	ShaderType_t GetShaderType( void ) const;
	void SetShaderType( RenderPass_t tRenderPass, ShaderType_t tShaderType );

private:
	ShaderType_t m_tShaderTypes[RENDERPASS_COUNT];
};

#endif // MATERIAL_H