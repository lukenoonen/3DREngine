#ifndef UTILDOUBLENODE_H
#define UTILDOUBLENODE_H

template <class T> class CUtilDoubleNode
{
public:
	CUtilDoubleNode<T>( const T &tData );
	CUtilDoubleNode<T>( const T &tData, CUtilDoubleNode<T> *pPrev, CUtilDoubleNode<T> *pNext );

	T &Peek( void );

	CUtilDoubleNode<T> *GetPrev( void );
	void SetPrev( CUtilDoubleNode<T> *pPrev );

	CUtilDoubleNode<T> *GetNext( void );
	void SetNext( CUtilDoubleNode<T> *pNext );

private:
	T m_tData;

	CUtilDoubleNode<T> *m_pPrev;
	CUtilDoubleNode<T> *m_pNext;
};

template <class T> CUtilDoubleNode<T>::CUtilDoubleNode<T>( const T &tData )
{
	m_tData = tData;
	m_pPrev = NULL;
	m_pNext = NULL;
}

template <class T> CUtilDoubleNode<T>::CUtilDoubleNode<T>( const T &tData, CUtilDoubleNode<T> *pPrev, CUtilDoubleNode<T> *pNext )
{
	m_tData = tData;
	m_pPrev = pPrev;
	m_pNext = pNext;
}

template <class T> T &CUtilDoubleNode<T>::Peek( void )
{
	return m_tData;
}

template <class T> CUtilDoubleNode<T> *CUtilDoubleNode<T>::GetPrev( void )
{
	return m_pPrev;
}

template <class T> void CUtilDoubleNode<T>::SetPrev( CUtilDoubleNode<T> *pPrev )
{
	m_pPrev = pPrev;
}

template <class T> CUtilDoubleNode<T> *CUtilDoubleNode<T>::GetNext( void )
{
	return m_pNext;
}

template <class T> void CUtilDoubleNode<T>::SetNext( CUtilDoubleNode<T> *pNext )
{
	m_pNext = pNext;
}

#endif // UTILDOUBLENODE_H