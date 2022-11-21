#ifndef BASEMODELED_H
#define BASEMODELED_H

#include "Global.h"
#include "BaseWorld.h"
#include "Model.h"

class CBaseModeled : public CBaseWorld
{
public:
	DECLARE_CLASS( CBaseModeled, CBaseWorld )

	DECLARE_DATADESC()

	CBaseModeled();

	virtual bool Init( void );

	virtual void PreDraw( void );
	virtual void Draw( void );

	virtual bool ShouldDraw( void ) const;

protected:
	CModel *GetModel( void );

private:
	CHandle<CModel> m_hModel;
};

#endif // BASEMODELED_H