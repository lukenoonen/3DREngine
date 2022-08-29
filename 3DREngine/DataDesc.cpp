#include "DataDesc.h"

CBaseDataField::CBaseDataField( const char *sName, unsigned int uiOffset )
{
	m_sName = sName;
	m_uiOffset = uiOffset;
}

bool CBaseDataField::Save( void *pData, CFile *pFile )
{
	return false;
}

bool CBaseDataField::Load( void *pData, CFile *pFile )
{
	return false;
}

bool CBaseDataField::Load( void *pData, CTextBlock *pTextBlock )
{
	return false;
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

bool CDataMap::Save( void *pData, CFile *pFile )
{
	for (unsigned int i = 0; i < m_pDataFields.size(); i++)
	{
		if (!m_pDataFields[i]->Save( pData, pFile ))
			return false;
	}

	if (m_pBaseMap)
		m_pBaseMap->Save( pData, pFile );
}

bool CDataMap::Load( void *pData, CFile *pFile )
{
	for (unsigned int i = 0; i < m_pDataFields.size(); i++)
	{
		if (!m_pDataFields[i]->Load( pData, pFile ))
			return false;
	}

	if (m_pBaseMap)
		m_pBaseMap->Load( pData, pFile );
}

bool CDataMap::Load( void *pData, CTextBlock *pTextBlock )
{
	for (unsigned int i = 0; i < m_pDataFields.size(); i++)
	{
		if (!m_pDataFields[i]->Load( pData, pTextBlock ))
			return false;
	}

	if (m_pBaseMap)
		m_pBaseMap->Load( pData, pTextBlock );
}