#ifndef UTILBUFFER_H
#define UTILBUFFER_H

#include <cstring>
#include <fstream>

class CUtilBuffer
{
public:
	void Begin( void );

	template <class T> bool Read( T &tData );
	template <class T> bool Write( T &tData );

private:
	char *m_pData;
	unsigned int m_uiSize;
	unsigned int m_uiPosition;
};

template <class T> bool CUtilBuffer::Read( T &tData )
{
	if (!m_pData)
		return false;

	unsigned int uiBytes = sizeof( T );
	if (m_uiPosition + uiBytes >= m_uiSize)
		return false;

	std::memcpy( &tData, &m_pData[m_uiPosition], uiBytes );

	m_uiPosition += uiBytes;

	return true;
}

template <class T> bool CUtilBuffer::Write( T &tData )
{
	if (!m_pData)
		return false;

	unsigned int uiBytes = sizeof( T );
	if (m_uiPosition + uiBytes >= m_uiSize)
		return false;

	std::memcpy( &m_pData[m_uiPosition], &tData, uiBytes );

	m_uiPosition += uiBytes;

	return true;
}

#endif // UTILBUFFER_H