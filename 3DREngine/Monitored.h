#ifndef MONITORED_H
#define MONITORED_H

#include "Global.h"

class CMonitored
{
public:
	DECLARE_CLASS_NOBASE( CMonitored )

	CMonitored();

	void Modify( void );
	bool Modified( void ) const;

private:
	unsigned long m_ulLastFrameModified;
};

template <class T> class CMonitoredValue : public CMonitored
{
public:
	DECLARE_CLASS( CMonitoredValue, CMonitored )

	CMonitoredValue();
	CMonitoredValue( T tValue );

	void Set( T tValue );
	const T &Get( void ) const;

	CMonitoredValue<T> &operator = ( T tValue );
	T *operator & ();

private:
	T m_tValue;
};

template <class T> CMonitoredValue<T>::CMonitoredValue()
{

}

template <class T> CMonitoredValue<T>::CMonitoredValue( T tValue )
{
	m_tValue = tValue;
}

template <class T> void CMonitoredValue<T>::Set( T tValue )
{
	m_tValue = tValue;
	Modify();
}

template<class T> CMonitoredValue<T> &CMonitoredValue<T>::operator = ( T tValue )
{
	Set( tValue );
	return *this;
}

template<class T> T *CMonitoredValue<T>::operator & ()
{
	return &m_tValue;
}

template <class T> const T &CMonitoredValue<T>::Get( void ) const
{
	return m_tValue;
}

template <class T> bool UTIL_Write( CFile *pFile, CMonitoredValue<T> &mData )
{
	T tValue = mData.Get();
	return pFile->Write( tValue );
}

template <class T> bool UTIL_Read( CFile *pFile, CMonitoredValue<T> &mData )
{
	T tValue;
	if (!pFile->Read( tValue )) return false;
	mData.Set( tValue );
	return true;
}

template <class T> bool UTIL_GetValue( const CTextItem *pTextItem, CMonitoredValue<T> &mValue )
{
	T tValue;
	if (!pTextItem->GetValue( tValue )) return false;
	mValue.Set( tValue );
	return true;
}

#endif // MONITORED_H