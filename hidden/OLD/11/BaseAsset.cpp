#include "BaseAsset.h"

EAssetType UTIL_AssetTypeExtensionToEnum( const char *sAssetTypeExtension )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)EAssetType::i_count; i++)
	{
		if (UTIL_streq( g_sAssetTypeExtensions[i], sAssetTypeExtension ))
			return (EAssetType)i;
	}

	return EAssetType::i_invalid;
}

const char *UTIL_AssetTypeEnumToExtension( EAssetType eAssetType )
{
	return g_sAssetTypeExtensions[(EBaseEnum)eAssetType];
}

CBaseAsset::CBaseAsset( const char *sPath )
{
	m_sPath = UTIL_stredit( sPath );
	m_uiActive = 0;
}

CBaseAsset::~CBaseAsset()
{
	delete[] m_sPath;
}

EAssetType CBaseAsset::GetAssetType( void ) const
{
	return EAssetType::i_invalid;
}

const char *CBaseAsset::GetPath( void ) const
{
	return m_sPath;
}

void CBaseAsset::Activate( void )
{
	m_uiActive++;
}

void CBaseAsset::Inactivate( void )
{
	m_uiActive--;
}

bool CBaseAsset::IsActive( void ) const
{
	return m_uiActive != 0;
}