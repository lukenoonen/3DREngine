#ifndef UTILSTACK_H
#define UTILSTACK_H

#include "UtilSingleNode.h"

template <class T> class CUtilStack
{
public:
	CUtilStack<T>();
	~CUtilStack<T>();

	void Clear( void );
	void Push( const T &tData );
	void Pop( void );
	T &Peek( void );
	bool IsEmpty( void );

private:
	CUtilSingleNode<T> *m_pFront;
};

template <class T> CUtilStack<T>::CUtilStack<T>()
{
	m_pFront = NULL;
}

template <class T> CUtilStack<T>::~CUtilStack<T>()
{
	Clear();
}

template <class T> void CUtilStack<T>::Clear( void )
{
	while (!IsEmpty())
	{
		delete Peek();
		Pop();
	}
}

template <class T> void CUtilStack<T>::Push( const T &tData )
{
	m_pFront = new CUtilSingleNode<T>( tData, m_pFront );
}

template <class T> void CUtilStack<T>::Pop( void )
{
	CUtilSingleNode<T> *pPrevFront = m_pFront;
	m_pFront = m_pFront->GetNext();
	delete pPrevFront;
}

template <class T> T &CUtilStack<T>::Peek( void )
{
	return m_pFront->Data();
}

template <class T> bool CUtilStack<T>::IsEmpty( void )
{
	return m_pFront == NULL;
}

#endif // UTILSTACK_H