#ifndef MATERIAL_H
#define MATERIAL_H

#include "Global.h"
#include "BaseAsset.h"
#include "RenderManager.h"

class CMaterial : public CBaseAsset
{
public:
	DECLARE_CLASS( CMaterial, CBaseAsset );

	CMaterial( const char *sPath );

	virtual EAssetType GetAssetType( void ) const;

	virtual void Use( void );

	bool ShouldDraw( void ) const;

	EShaderType GetShaderType( void ) const;
	void SetShaderType( ERenderPass eRenderPass, EShaderType eShaderType );

private:
	EShaderType m_tShaderTypes[(EBaseEnum)ERenderPass::i_count];
};

#endif // MATERIAL_H