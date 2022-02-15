#ifndef BASEMODELED_H
#define BASEMODELED_H

#include "Global.h"
#include "BaseWorld.h"
#include "Geometry.h"
#include "Material.h"

class CBaseModeled : public CBaseWorld
{
public:
	DECLARE_CLASS( CBaseModeled, CBaseWorld );

	CBaseModeled();

	virtual void Exit( void );

	virtual void PreDraw( void );
	virtual void Draw( void );

	virtual bool ShouldDraw( void ) const;

	void AddGeometry( const char *sPath );
	void AddMaterial( const char *sPath );

private:
	std::vector<CGeometry *> m_pGeometry;
	std::vector<CMaterial *> m_pMaterials;

	bool m_bShouldDraw[(EBaseEnum)ERenderPass::i_count];
};

#endif // BASEMODELED_H