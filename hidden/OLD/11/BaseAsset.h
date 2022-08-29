#ifndef BASEASSET_H
#define BASEASSET_H

#include "Global.h"

enum class EAssetType : EBaseEnum
{
	t_animation = 0,
	t_geometry,
	// t_material,
	t_skeleton,
	t_texture,

	i_count,
	i_invalid = i_count,
};

static const char *g_sAssetTypeExtensions[] =
{
	"3an",
	"3gm",
	// "3mt",
	"3tx",
	"3sk",
};

EAssetType UTIL_AssetTypeExtensionToEnum( const char *sAssetTypeExtension );
const char *UTIL_AssetTypeEnumToExtension( EAssetType eAssetType );

class CBaseAsset
{
public:
	DECLARE_CLASS_NOBASE( CBaseAsset );

	CBaseAsset( const char *sPath );
	virtual ~CBaseAsset();

	virtual EAssetType GetAssetType( void ) const;

	const char *GetPath( void ) const;

	void Activate( void );
	void Inactivate( void );
	bool IsActive( void ) const;

private:
	char *m_sPath;
	unsigned int m_uiActive;
};

#endif // BASEASSET_H