#ifndef ANY_H
#define ANY_H

#include "Global.h"

class CBaseAny
{
public:
	DECLARE_CLASS_NOBASE( CBaseAny )

	CBaseAny();
	virtual ~CBaseAny();

	template <class T> bool Get( T &tValue ) const;
	template <class T> bool Set( T tValue );
};

template <class T> class CAny : public CBaseAny
{
public:
	DECLARE_CLASS( CAny<T>, CBaseAny )

	CAny();
	CAny( T tValue );

	void GetInternal( T &tValue ) const;
	void SetInternal( T tValue );

private:
	T m_tValue;
};

template <class T> bool CBaseAny::Get( T &tValue ) const
{
	const CAny<T> *pThis = dynamic_cast<const CAny<T> *>(this);
	if (!pThis)
		return false;

	pThis->GetInternal( tValue );
	return true;
}

template <class T> bool CBaseAny::Set( T tValue )
{
	CAny<T> *pThis = dynamic_cast<CAny<T> *>(this);
	if (!pThis)
		return false;

	pThis->SetInternal( tValue );
	return true;
}

template <class T> CAny<T>::CAny()
{

}

template <class T> CAny<T>::CAny( T tValue )
{
	m_tValue = tValue;
}

template <class T> void CAny<T>::GetInternal( T &tValue ) const
{
	tValue = m_tValue;
}

template <class T> void CAny<T>::SetInternal( T tValue )
{
	m_tValue = tValue;
}

#endif // ANY_H