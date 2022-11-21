#include "DataDesc.h"

CBaseDataField::CBaseDataField( const char *sName, unsigned int uiOffset, int iFlags )
{
	m_sName = sName;
	m_uiOffset = uiOffset;
	m_iFlags = iFlags;
}

bool CBaseDataField::Save( void *pData ) const
{
	return SaveInternal( pData );
}

bool CBaseDataField::Load( void *pData ) const
{
	return LoadInternal( pData );
}

bool CBaseDataField::LoadText( void *pData, const CTextBlock *pTextBlock ) const
{
	return LoadTextInternal( pData, pTextBlock ) || !(m_iFlags & FL_REQUIRED);
}

bool CBaseDataField::Link( void *pData ) const
{
	return LinkInternal( pData ) || !(m_iFlags & FL_REQUIRED);
}

bool CBaseDataField::SaveInternal( void *pData ) const
{
	return true;
}

bool CBaseDataField::LoadInternal( void *pData ) const
{
	return true;
}

bool CBaseDataField::LoadTextInternal( void *pData, const CTextBlock *pTextBlock ) const
{
	return true;
}

bool CBaseDataField::LinkInternal( void *pData ) const
{
	return true;
}

CDataMap::CDataMap()
{
	m_pBaseMap = NULL;
}

void CDataMap::AddDataField( CBaseDataField *pDataField )
{
	m_pDataFields.push_back( pDataField );
}

void CDataMap::SetBaseMap( CDataMap *pBaseMap )
{
	m_pBaseMap = pBaseMap;
}

bool CDataMap::Save( void *pData ) const
{
	for (unsigned int i = 0; i < m_pDataFields.size(); i++)
	{
		if (!m_pDataFields[i]->Save( pData ))
			return false;
	}

	if (m_pBaseMap)
		return m_pBaseMap->Save( pData );

	return true;
}

bool CDataMap::Load( void *pData ) const
{
	for (unsigned int i = 0; i < m_pDataFields.size(); i++)
	{
		if (!m_pDataFields[i]->Load( pData ))
			return false;
	}

	if (m_pBaseMap)
		return m_pBaseMap->Load( pData );

	return true;
}

bool CDataMap::LoadText( void *pData, const CTextBlock *pTextBlock ) const
{
	for (unsigned int i = 0; i < m_pDataFields.size(); i++)
	{
		if (!m_pDataFields[i]->LoadText( pData, pTextBlock ))
			return false;
	}

	if (m_pBaseMap)
		return m_pBaseMap->LoadText( pData, pTextBlock );

	return true;
}

bool CDataMap::Link( void *pData ) const
{
	for (unsigned int i = 0; i < m_pDataFields.size(); i++)
	{
		if (!m_pDataFields[i]->Link( pData ))
			return false;
	}

	if (m_pBaseMap)
		return m_pBaseMap->Link( pData );

	return true;
}