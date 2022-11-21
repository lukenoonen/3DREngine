#ifndef CSMLIGHT_H
#define CSMLIGHT_H

#include "Global.h"
#include "BaseLight.h"

class CCSMLight : public CBaseLight
{
public:
	DECLARE_CLASS( CCSMLight, CBaseLight );

	DECLARE_LINKED_CLASS()

	CCSMLight();

	virtual bool Init( void );

	virtual void ActivateLight( void );
};

#endif // CSMLIGHT_H