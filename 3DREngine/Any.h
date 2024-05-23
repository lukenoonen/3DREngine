#ifndef ANY_H
#define ANY_H

#include "Global.h"

class CBaseAny
{
public:
	DECLARE_CLASS_NOBASE( CBaseAny )

	CBaseAny();

	template <class T> bool Get( T &tValue ) const;
	template <class T> bool Set( T tValue );
};

template <class T> class CAny : public CBaseAny
{
public:
	DECLARE_CLASS( CAny<T>, CBaseAny )

	CAny();
	CAny( T tValue );

	void GetValue( T &tValue ) const;
	void SetValue( T tValue );

private:
	T m_tValue;
};

template <class T> bool CBaseAny::Get( T &tValue ) const
{
	CAny<T> *pThis = dynamic_cast<CAny<T> *>(this);
	if (!pThis)
		return false;

	pThis->GetValue( tValue );
	return true;
}

template <class T> bool CBaseAny::Set( T tValue )
{
	CAny<T> *pThis = dynamic_cast<CAny<T> *>(this);
	if (!pThis)
		return false;

	pThis->SetValue( tValue );
	return true;
}

template <class T> CAny<T>::CAny()
{

}

template <class T> CAny<T>::CAny( T tValue )
{
	m_tValue = tValue;
}

template <class T> void CAny<T>::GetValue( T &tValue ) const
{
	tValue = m_tValue;
}

template <class T> void CAny<T>::SetValue( T tValue )
{
	m_tValue = tValue;
}

#endif // ANY_H