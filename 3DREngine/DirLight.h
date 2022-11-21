#ifndef DIRLIGHT_H
#define DIRLIGHT_H

#include "Global.h"
#include "BaseLight.h"

class CDirLight : public CBaseLight
{
public:
	DECLARE_CLASS( CDirLight, CBaseLight )

	DECLARE_LINKED_CLASS()

	CDirLight();

	virtual bool Init( void );

	virtual void ActivateLight( void );
};

#endif // DIRLIGHT_H