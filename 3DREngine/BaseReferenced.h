#ifndef BASEREFERENCED_H
#define BASEREFERENCED_H

#include "Global.h"

class CBaseReferenced
{
public:
	DECLARE_CLASS_NOBASE( CBaseReferenced )

	CBaseReferenced();

	void Reference( void );
	void Unreference( void );
	bool IsReferenced( void ) const;

private:
	unsigned int m_uiReferences;
};

#endif // BASEREFERENCED_H