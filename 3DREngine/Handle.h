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
	CBaseHandle( CBaseHandle &hOther );
	virtual ~CBaseHandle();

	void SetEntity( CBaseEntity *pEntity );
	void SetName( const char *sName );
	void SetIndex( unsigned int uiIndex );

	CBaseEntity *Get( void ) const;
	bool Check( void );

	bool Link( void );

protected:
	virtual bool Verify( CBaseEntity *pEntity ) const;

	void CopyOther( CBaseHandle &hOther );

private:
	CBaseEntity *FindEntity( void ) const;

	void OnChangeData( void );

	void SetEntityInternal( CBaseEntity *pEntity );
	void SetNameInternal( const char *sName );
	void SetIndexInternal( unsigned int uiIndex );

private:
	unsigned char m_ucActiveData; // TODO: maybe make this an enum
	union
	{
		CBaseEntity *pEntity;
		char *sName;
		unsigned int uiIndex;
	} m_uData;
};

bool UTIL_Write( CFile *pFile, CBaseHandle &hData );
bool UTIL_Read( CFile *pFile, CBaseHandle &hData );
bool UTIL_GetValue( const CTextItem *pTextItem, CBaseHandle &hValue );

template <class T> class CHandle : public CBaseHandle
{
public:
	DECLARE_CLASS( CHandle<T>, CBaseHandle )

	CHandle();
	CHandle( T *pEntity );
	CHandle( CHandle<T> &hOther );

	T *Get( void ) const;

	operator T *();
	operator T *() const;
	bool operator ! () const;
	bool operator == ( T *val ) const;
	bool operator != ( T *val ) const;
	CHandle<T> &operator = ( T *val );
	T *operator -> () const;

	CHandle<T> &operator = ( CHandle<T> &hOther );

protected:
	virtual bool Verify( CBaseEntity *pEntity ) const;
};

template <class T> CHandle<T>::CHandle()
{

}

template <class T> CHandle<T>::CHandle( T *pEntity ) : BaseClass( pEntity )
{

}

template <class T> CHandle<T>::CHandle( CHandle<T> &hOther ) : BaseClass( hOther )
{

}

template <class T> T *CHandle<T>::Get( void ) const
{
	return static_cast<T *>(BaseClass::Get());
}

template <class T> bool CHandle<T>::Verify( CBaseEntity *pEntity ) const
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

template<class T> CHandle<T> &CHandle<T>::operator = ( T *pEntity )
{
	SetEntity( pEntity );
	return *this;
}

template<class T> T *CHandle<T>::operator -> () const
{
	return Get();
}

template<class T> CHandle<T> &CHandle<T>::operator = ( CHandle<T> &hOther )
{
	CopyOther( hOther );
	return *this;
}

template <class T> bool UTIL_Write( CFile *pFile, CHandle<T> &hData )
{
	return UTIL_Write( pFile, (CBaseHandle &)hData );
}

template <class T> bool UTIL_Read( CFile *pFile, CHandle<T> &hData )
{
	return UTIL_Read( pFile, (CBaseHandle &)hData );
}

template <class T> bool UTIL_GetValue( const CTextItem *pTextItem, CHandle<T> &hValue )
{
	return UTIL_GetValue( pTextItem, (CBaseHandle &)hValue );
}

#endif // HANDLE_H