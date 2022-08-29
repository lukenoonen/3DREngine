#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "Global.h"
#include "Model.h"

enum GenericModels_t : unsigned char
{
	GENERICMODELS_CUBE = 0,
	GENERICMODELS_INVERSECUBE,
	GENERICMODELS_PLANE,
	GENERICMODELS_COUNT,
};

static const char *g_sGenericModelPaths[] =
{
	"cube.3md",
	"inversecube.3md",
	"plane.3md",
};

class CModelManager
{
public:
	DECLARE_CLASS_NOBASE( CModelManager );

	CModelManager();
	~CModelManager();

	void Reset( void );

	CModel *GetModel( const char *sPath );
	CModel *GetGenericModel( GenericModels_t tGenericModel );

private:
	CModel *CreateModel( const char *sPath );

	std::vector<CModel *> m_pModels;
	CModel *m_pGenericModels[GENERICMODELS_COUNT];
};

#endif // MODELMANAGER_H