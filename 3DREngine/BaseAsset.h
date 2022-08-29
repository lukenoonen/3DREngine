#ifndef BASEASSET_H
#define BASEASSET_H

#include "Global.h"
#include "BaseEntity.h"

class CBaseAsset : public CBaseEntity
{
public:
	DECLARE_CLASS( CBaseAsset, CBaseEntity )

	CBaseAsset();
};

#endif // BASEASSET_H