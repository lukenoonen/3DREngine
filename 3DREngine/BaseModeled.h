#ifndef BASEMODELED_H
#define BASEMODELED_H

#include "Global.h"
#include "BaseWorld.h"
#include "Model.h"

class CBaseModeled : public CBaseWorld
{
public:
	DECLARE_CLASS( CBaseModeled, CBaseWorld )

	CBaseModeled();

	virtual void PreDraw( void );
	virtual void Draw( void );

	virtual bool ShouldDraw( void ) const;

	void SetModel( CModel *pModel );

protected:
	CModel *GetModel( void );

private:
	CModel *m_pModel;
};

#endif // BASEMODELED_H