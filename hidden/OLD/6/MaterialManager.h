#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "Global.h"
#include "Material.h"
#include "MaterialController.h"

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

	std::vector<CMaterial *> m_pMaterials;
	std::vector<CMaterialController *> m_pMaterialControllers;
};

#endif // MATERIALMANAGER_H