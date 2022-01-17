#ifndef BASEMODELED_H
#define BASEMODELED_H

#include "Global.h"
#include "BaseWorld.h"
#include "Model.h"

class CBaseModeled : public CBaseWorld
{
public:
	DECLARE_CLASS( CBaseModeled, CBaseWorld );

	CBaseModeled();

	virtual void PreDraw( void );
	virtual void Draw( void );

	virtual void SetModel( CModel *pModel );

	CModel *GetModel( void ) const;

	virtual bool ShouldDraw( void ) const;

private:
	CModel *m_pModel;
};

#endif // BASEMODELED_H