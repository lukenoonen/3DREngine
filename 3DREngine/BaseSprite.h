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

	CBaseSprite( const char *sMaterialPath, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
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