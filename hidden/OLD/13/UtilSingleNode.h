#ifndef UTILSINGLENODE_H
#define UTILSINGLENODE_H

template <class T> class CUtilSingleNode
{
public:
	CUtilSingleNode<T>( const T &tData );
	CUtilSingleNode<T>( const T &tData, CUtilSingleNode<T> *pNext );

	T &Peek( void );

	CUtilSingleNode<T> *GetNext( void );
	void SetNext( CUtilSingleNode<T> *pNext );

private:
	T m_tData;

	CUtilSingleNode<T> *m_pNext;
};

template <class T> CUtilSingleNode<T>::CUtilSingleNode<T>( const T &tData )
{
	m_tData = tData;
	m_pNext = NULL;
}

template <class T> CUtilSingleNode<T>::CUtilSingleNode<T>( const T &tData, CUtilSingleNode<T> *pNext )
{
	m_tData = tData;
	m_pNext = pNext;
}

template <class T> T &CUtilSingleNode<T>::Peek( void )
{
	return m_tData;
}

template <class T> CUtilSingleNode<T> *CUtilSingleNode<T>::GetNext( void )
{
	return m_pNext;
}

template <class T> void CUtilSingleNode<T>::SetNext( CUtilSingleNode<T> *pNext )
{
	m_pNext = pNext;
}

#endif // UTILSINGLENODE_H