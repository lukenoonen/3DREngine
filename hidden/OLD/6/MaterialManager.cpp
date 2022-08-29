#include "MaterialManager.h"
#include <fstream>
#include "GlobalManager.h"
#include "ShaderManager.h"

#include "LitMaterial.h"
#include "UnlitMaterial.h"
#include "SkyboxMaterial.h"

#include <UTIL.h>

CMaterialManager::CMaterialManager()
{

}

CMaterialManager::~CMaterialManager()
{
	Reset();
}

void CMaterialManager::Reset( void )
{
	for (unsigned int i = 0; i < m_pMaterials.size(); i++)
		delete m_pMaterials[i];

	for (unsigned int i = 0; i < m_pMaterialControllers.size(); i++)
		delete m_pMaterialControllers[i];

	m_pMaterials.clear();

	m_pMaterialControllers.clear();
}

CMaterialController *CMaterialManager::GetMaterialController( const char *sPath )
{
	if (UTIL_streq( UTIL_extn( sPath ), "3mt" ))
	{
		std::vector<CMaterial *> pMaterials;
		pMaterials.push_back( GetMaterial( sPath ) );
		CMaterialController *pNewMaterialController = new CMaterialController( pMaterials );
		m_pMaterialControllers.push_back( pNewMaterialController );
		return pNewMaterialController;
	}

	char sFullPath[260];
	sprintf_s( sFullPath, sizeof( sFullPath ), "resources/materials/%s", sPath );

	std::fstream fMaterialController( sFullPath, std::ios::in | std::ios::binary );
	if (!fMaterialController.is_open())
		ExitError( "Cannot open material controller file." );

	std::vector<CMaterial *> pMaterials;
	
	unsigned int uiMaterialsCount;
	UTIL_Read( fMaterialController, &uiMaterialsCount, 1, unsigned int );

	for (unsigned int i = 0; i < uiMaterialsCount; i++)
	{
		char *sMaterialName;
		UTIL_ReadString( fMaterialController, sMaterialName );
		pMaterials.push_back( GetMaterial( sMaterialName ) );
		delete[] sMaterialName;
	}

	CMaterialController *pNewMaterialController = new CMaterialController( pMaterials );
	m_pMaterialControllers.push_back( pNewMaterialController );
	return pNewMaterialController;
}

CMaterial *CMaterialManager::GetMaterial( const char *sPath )
{
	for (unsigned int i = 0; i < m_pMaterials.size(); i++)
	{
		if (UTIL_strcmp( m_pMaterials[i]->GetPath(), sPath ) == 0)
			return m_pMaterials[i];
	}

	CMaterial *pNewMaterial = CreateMaterial( sPath );

	m_pMaterials.push_back( pNewMaterial );
	return pNewMaterial;
}

CMaterial *CMaterialManager::CreateMaterial( const char *sPath )
{
	char sFullPath[260];
	sprintf_s( sFullPath, sizeof(sFullPath), "resources/materials/%s", sPath );

	std::fstream fMaterial( sFullPath, std::ios::in | std::ios::binary );
	if (!fMaterial.is_open())
		ExitError( "Cannot open material file." );

	CMaterial *pNewMaterial = NULL;
	
	ShaderType_t tShaderType;
	UTIL_Read( fMaterial, &tShaderType, 1, ShaderType_t );

	switch (tShaderType)
	{
	case SHADERTYPE_LIT:
	{
		CTexture *pDiffuse;
		CTexture *pSpecular;
		CTexture *pNormal;
		float flShininess;
		glm::vec2 vecTextureScale;
		unsigned int uiDrawFlags;

		char *sTexturePath;

		UTIL_ReadString( fMaterial, sTexturePath );
		pDiffuse = pTextureManager->GetTexture( sTexturePath );
		delete[] sTexturePath;

		UTIL_ReadString( fMaterial, sTexturePath );
		pSpecular = pTextureManager->GetTexture( sTexturePath );
		delete[] sTexturePath;

		UTIL_ReadString( fMaterial, sTexturePath );
		pNormal = pTextureManager->GetTexture( sTexturePath );
		delete[] sTexturePath;
		
		UTIL_Read( fMaterial, &flShininess, 1, float );

		UTIL_Read( fMaterial, &vecTextureScale, 1, glm::vec2 );

		UTIL_Read( fMaterial, &uiDrawFlags, 1, unsigned int );

		pNewMaterial = new CLitMaterial( pDiffuse, pSpecular, pNormal, flShininess, vecTextureScale, sPath, uiDrawFlags );
		break;
	}
	case SHADERTYPE_UNLIT:
	{
		CTexture *pDiffuse;
		glm::vec2 vecTextureScale;
		unsigned int uiDrawFlags;

		char *sTexturePath;

		UTIL_ReadString( fMaterial, sTexturePath );
		pDiffuse = pTextureManager->GetTexture( sTexturePath );
		delete[] sTexturePath;

		UTIL_Read( fMaterial, &vecTextureScale, 1, glm::vec2 );

		UTIL_Read( fMaterial, &uiDrawFlags, 1, unsigned int );

		pNewMaterial = new CUnlitMaterial( pDiffuse, vecTextureScale, sPath, uiDrawFlags );
		break;
	}
	case SHADERTYPE_SKYBOX:
	{
		CTexture *pSkybox;
		unsigned int uiDrawFlags;

		char *sTexturePath;

		UTIL_ReadString( fMaterial, sTexturePath );
		pSkybox = pTextureManager->GetTexture( sTexturePath );
		delete[] sTexturePath;

		UTIL_Read( fMaterial, &uiDrawFlags, 1, unsigned int );

		pNewMaterial = new CSkyboxMaterial( pSkybox, sPath, uiDrawFlags );
		break;
	}
	case SHADERTYPE_COLORHUD:
	{
		glm::vec3 vecColor;
		unsigned int uiDrawFlags;

		UTIL_Read( fMaterial, &vecColor, 1, glm::vec3 );
		UTIL_Read( fMaterial, &uiDrawFlags, 1, unsigned int );

		pNewMaterial = new CColorHUDMaterial( vecColor, sPath, uiDrawFlags );
		break;
	}
	}

	return pNewMaterial;
}