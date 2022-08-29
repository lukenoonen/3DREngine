#ifndef HANDLE_H
#define HANDLE_H

#include "Global.h"

class CBaseEntity;

class CBaseHandle
{
public:
	DECLARE_CLASS_NOBASE( CBaseHandle )

	CBaseHandle();
	CBaseHandle( CBaseEntity *pEntity );

	void SetEntity( CBaseEntity *pEntity );
	void SetName( const char *sName );
	void SetIndex( unsigned int uiIndex );

	CBaseEntity *Get( void );

	virtual bool Verify( CBaseEntity *pEntity );

private:
	unsigned char m_ucActiveData; // TODO: maybe make this an enum
	union
	{
		CBaseEntity *pEntity;
		char *sName;
		unsigned int uiIndex;
	} m_uData;
};

template <class T> class CHandle : public CBaseHandle
{
public:
	DECLARE_CLASS( CHandle<T>, CBaseHandle )

	CHandle();
	CHandle( T *pEntity );

	T *Get( void );

	virtual bool Verify( CBaseEntity *pEntity );

	operator T *();
	operator T *() const;
	bool operator ! () const;
	bool operator == ( T *val ) const;
	bool operator != ( T *val ) const;
	CHandle<T> &operator = ( const T *val );
	T *operator -> () const;
};

template <class T> CHandle<T>::CHandle()
{

}

template <class T> CHandle<T>::CHandle( T *pEntity ) : BaseClass( pEntity )
{

}

template <class T> T *CHandle<T>::Get( void )
{
	return static_cast<T *>(BaseClass::Get());
}

template <class T> bool CHandle<T>::Verify( CBaseEntity *pEntity )
{
	return BaseClass::Verify( pEntity ) && dynamic_cast<T *>(pEntity) != NULL;
}

template<class T> CHandle<T>::operator T *()
{
	return Get();
}

template<class T> CHandle<T>::operator T *() const
{
	return Get();
}

template<class T> bool CHandle<T>::operator ! () const
{
	return !Get();
}

template<class T> bool CHandle<T>::operator == ( T *pEntity ) const
{
	return Get() == pEntity;
}

template<class T> bool CHandle<T>::operator != ( T *pEntity ) const
{
	return Get() != pEntity;
}

template<class T> CHandle<T> &CHandle<T>::operator = ( const T *pEntity )
{
	SetEntity( pEntity );
	return *this;
}

template<class T> T *CHandle<T>::operator -> () const
{
	return Get();
}

#endif // HANDLE_H