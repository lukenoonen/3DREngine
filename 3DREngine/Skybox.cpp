#include "Skybox.h"
#include "RenderManager.h"

DEFINE_DATADESC( CSkybox )

	DEFINE_FIELD( LinkedDataField, CHandle<CSkyboxMaterial>, m_hSkyboxMaterial, "skyboxmaterial", FL_REQUIRED )
	DEFINE_FIELD( LinkedDataField, CHandle<CGeometry>, m_hGeometry, "geometry", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CSkybox, skybox )

CSkybox::CSkybox()
{

}

void CSkybox::PreDraw( void )
{
	m_hSkyboxMaterial->Apply();
}

void CSkybox::Draw( void )
{
	m_hGeometry->Draw();
}

bool CSkybox::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_hSkyboxMaterial->ShouldApply();
}