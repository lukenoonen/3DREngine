#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "Global.h"
#include "Material.h"
#include "MaterialController.h"

enum GenericMaterials_t
{
	GENERICMATERIAL_ERROR = 0,
	GENERICMATERIAL_COUNT,
};

static const char *g_sGenericMaterialPaths[] =
{
	"error.3mt",
};

class CMaterialManager
{
public:
	DECLARE_CLASS_NOBASE( CMaterialManager );

	CMaterialManager();
	~CMaterialManager();

	void Reset( void );

	CMaterialController *GetMaterialController( const char *sPath );

	CMaterial *GetMaterial( const char *sPath );

private:
	CMaterial *CreateMaterial( const char *sPath );

	CMaterial *m_pGenericMaterials[GENERICMATERIAL_COUNT];
	std::vector<CMaterial *> m_pMaterials;
	std::vector<CMaterialController *> m_pMaterialControllers;
};

#endif // MATERIALMANAGER_H