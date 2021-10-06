#ifndef BASEMODELED_H
#define BASEMODELED_H

#include "Global.h"
#include "BaseWorld.h"
#include "Model.h"

class CBaseModeled : public CBaseWorld
{
public:
	DECLARE_CLASS( CBaseModeled, CBaseWorld );

	CBaseModeled( const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void PreDraw( void );
	virtual void Draw( void );

	virtual void SetModel( CModel *pModel );

	CModel *GetModel( void ) const;

	virtual bool ShouldDraw( void ) const;

private:
	CModel *m_pModel;
};

#endif // BASEMODELED_H