#include "Cubemap.h"

DEFINE_DATADESC( CCubemap )

	DEFINE_FIELD( DataField, CCubemapResource *, m_pCubemapResource, "cubemapresource", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CCubemap, cubemap )

CCubemap::CCubemap()
{

}

int CCubemap::Bind( void ) const
{
	return m_pCubemapResource->Bind();
}