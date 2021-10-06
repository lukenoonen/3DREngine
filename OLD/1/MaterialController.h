#ifndef MATERIALCONTROLLER_H
#define MATERIALCONTROLLER_H

#include "Global.h"
#include "BaseController.h"
#include "Material.h"

typedef CBaseControllerSequence<CMaterial> CMaterialControllerSequence;
typedef CBaseController<CMaterial> CMaterialController;

/*#include "Global.h"
#include "Material.h"

class CMaterialController
{
public:
	DECLARE_CLASS_NOBASE( CMaterialController );

	CMaterialController( const std::vector<CMaterial *> &pMaterials );

	bool Use( void );
	bool Use( unsigned int uiIndex );

	bool ShouldDraw( unsigned int uiDrawFlags ) const;
	bool ShouldDraw( unsigned int uiIndex, unsigned int uiDrawFlags ) const;

	unsigned int GetMaterialCount( void ) const;

private:
	std::vector<CMaterial *> m_pMaterials;
	unsigned int m_uiSummedDrawFlags;
};*/

#endif // MATERIALCONTROLLER_H