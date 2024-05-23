#ifndef DATAMAP_H
#define DATAMAP_H

#include "Global.h"
#include "Any.h"

class CKeyValues
{
public:
	DECLARE_CLASS_NOBASE( CKeyValues )

	CKeyValues();
	~CKeyValues();

	template <class T> void Insert( const char *sKey, T tValue );
	template <class T> bool Get( const char *sKey, T &tValue );

private:
	std::unordered_map<const char *, CBaseAny *> m_mKeyValues;
};

template <class T> void CKeyValues::Insert( const char *sKey, T tValue )
{
	std::unordered_map<const char *, CBaseAny *>::iterator itSearch = m_mKeyValues.find( sKey );
	if (itSearch != m_mKeyValues.end())
		delete itSearch->second;

	m_mKeyValues[sKey] = new CAny<T>( tValue );
}

template <class T> bool CKeyValues::Get( const char *sKey, T &tValue )
{
	std::unordered_map<const char *, CBaseAny *>::iterator itSearch = m_mKeyValues.find( sKey );
	if (itSearch != m_mKeyValues.end())
		return itSearch->second->Get( tValue );

	return false;
}

#endif // DATAMAP_H