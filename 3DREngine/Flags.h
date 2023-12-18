#ifndef FLAGS_H
#define FLAGS_H

#include "Global.h"

template <class T> class CFlags
{
public:
	DECLARE_CLASS_NOBASE( CFlags<T> )

	CFlags();

	void AddFlag( T tFlag );
	void RemoveFlag( T tFlag );
	void ClearFlags( void );

	bool HasFlag( T tFlag ) const;

private:
	int m_iFlags;
};

template <class T> CFlags<T>::CFlags()
{
	m_iFlags = 0;
}

template <class T> void CFlags<T>::AddFlag( T tFlag )
{
	m_iFlags |= (1 << (int)tFlag);
}

template <class T> void CFlags<T>::RemoveFlag( T tFlag )
{
	m_iFlags &= ~(1 << (int)tFlag);
}

template <class T> void CFlags<T>::ClearFlags( void )
{
	m_iFlags = 0;
}

template <class T> bool CFlags<T>::HasFlag( T tFlag ) const
{
	return m_iFlags & (1 << (int)tFlag);
}

template <class T> bool UTIL_GetValue( const CTextItem *pTextItem, CFlags<T> &fValue )
{
	CTextLine *pTextLine;
	if (!pTextItem->GetValue( pTextLine ))
		return false;

	for (unsigned int i = 0; i < pTextLine->GetTextItemCount(); i++)
	{
		const char *sName;
		if (!pTextLine->GetTextItem( i )->GetValue( sName ))
			return false;

		T tValue = UTIL_EnumNameToValue<T>( sName );
		if (tValue == T::i_invalid)
			return false;

		fValue.AddFlag( tValue );
	}

	return true;
}

#endif // FLAGS_H