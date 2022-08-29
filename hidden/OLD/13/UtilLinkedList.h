#ifndef UTILLINKEDLIST_H
#define UTILLINKEDLIST_H

#include "UtilSingleNode.h"
#include "UtilStack.h"

// TODO: verify that these data structures (not just this one) works

template <class T> class CUtilLinkedList
{
public:
	CUtilLinkedList<T>();
	~CUtilLinkedList<T>();

	void Clear( void );
	void Add( const T &tData );
	bool Remove( const T &tData );

	void Start( void );
	void Iterate( void );
	void Interrupt( void );
	bool Continue( void );
	T &Get( void );

	bool IsEmpty( void );

private:
	CUtilSingleNode<T> *m_pFront;
	CUtilStack<CUtilSingleNode<T> *> m_pIterationStack;
};

template <class T> CUtilLinkedList<T>::CUtilLinkedList<T>()
{
	m_pFront = NULL;
}

template <class T> CUtilLinkedList<T>::~CUtilLinkedList<T>()
{
	Clear();
}

template <class T> void CUtilLinkedList<T>::Clear( void )
{
	for (Start(); Continue(); Iterate())
		delete Get();
}

template <class T> void CUtilLinkedList<T>::Add( const T &tData )
{
	m_pFront = new CUtilSingleNode<T>( tData, m_pFront );
}

template <class T> bool CUtilLinkedList<T>::Remove( const T &tData )
{
	if (m_pFront->Peek() == tData)
	{
		CUtilSingleNode<T> *pPrevFront = m_pFront;
		m_pFront = m_pFront->GetNext();
		delete pPrevFront;
		return true;
	}

	CUtilSingleNode<T> *pPrevSearch = m_pFront;
	CUtilSingleNode<T> *pSearch = m_pFront->GetNext();
	while (pSearch != NULL)
	{
		if (pSearch->Peek() == tData)
		{
			pPrevSearch->SetNext( pSearch->GetNext() );
			delete pSearch;
			return true;
		}

		pPrevSearch = pSearch;
		pSearch = pSearch->GetNext();
	}

	return false;
}

template <class T> void CUtilLinkedList<T>::Start( void )
{
	m_pIterationStack.Push( m_pFront );
}

template <class T> void CUtilLinkedList<T>::Iterate( void )
{
	CUtilSingleNode<T> *&pFront = m_pIterationStack.Peek();
	pFront = pFront->Next();
}

template <class T> void CUtilLinkedList<T>::Interrupt( void )
{
	m_pIterationStack.Pop();
}

template <class T> bool CUtilLinkedList<T>::Continue( void )
{
	return m_pIterationStack.Peek() != NULL;
}

template <class T> T &CUtilLinkedList<T>::Get( void )
{
	return m_pIterationStack.Peek()->Peek();
}

template <class T> bool CUtilLinkedList<T>::IsEmpty( void )
{
	return m_pFront == NULL;
}

#endif // UTILLINKEDLIST_H