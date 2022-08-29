#ifndef UTILQUEUE_H
#define UTILQUEUE_H

#include "UtilSingleNode.h"

template <class T> class CUtilQueue
{
public:
	CUtilQueue<T>();
	~CUtilQueue<T>();

	void Clear( void );
	void Push( const T &tData );
	void Pop( void );
	T &Peek( void );
	bool IsEmpty( void );

private:
	CUtilSingleNode<T> *m_pFront;
	CUtilSingleNode<T> *m_pBack;
};

template <class T> CUtilQueue<T>::CUtilQueue<T>()
{
	m_pFront = NULL;
	m_pBack = NULL;
}

template <class T> CUtilQueue<T>::~CUtilQueue<T>()
{
	Clear();
}

template <class T> void CUtilQueue<T>::Clear( void )
{
	while (!IsEmpty())
	{
		delete Peek();
		Pop();
	}
}

template <class T> void CUtilQueue<T>::Push( const T &tData )
{
	CUtilSingleNode<T> *pNewBack = new CUtilSingleNode<T>( tData );
	if (!m_pFront)
	{
		m_pFront = pNewBack;
		m_pBack = pNewBack;
	}
	else
	{
		m_pBack->SetNext( pNewBack );
		m_pBack = pNewBack;
	}
}

template <class T> void CUtilQueue<T>::Pop( void )
{
	if (m_pFront == m_pBack)
	{
		delete m_pFront;
		delete m_pBack;
		m_pFront = NULL;
		m_pBack = NULL;
	}
	else
	{
		CUtilSingleNode<T> *pPrevFront = m_pFront;
		m_pFront = m_pFront->GetNext();
		delete pPrevFront;
	}
}

template <class T> T &CUtilQueue<T>::Peek( void )
{
	return m_pFront->Data();
}

template <class T> bool CUtilQueue<T>::IsEmpty( void )
{
	return m_pFront == NULL;
}

#endif // UTILQUEUE_H