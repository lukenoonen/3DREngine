#ifndef BASESPRITE_H
#define BASESPRITE_H

#include "Global.h"
#include "BaseWorld.h"
#include "Material.h"
#include "Geometry.h"

class CBaseSprite : public CBaseWorld
{
public:
	DECLARE_CLASS( CBaseSprite, CBaseWorld );

	CBaseSprite( const char *sMaterialPath );
	virtual ~CBaseSprite();

	virtual void PostThink( void );

	virtual void PreDraw( void );
	virtual void Draw( void );

	virtual bool ShouldDraw( void ) const;

private:
	CMaterial *m_pMaterial;
	CGeometry *m_pGeometry;
};

#endif // BASESPRITE_H