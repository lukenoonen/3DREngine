#ifndef MATERIAL_H
#define MATERIAL_H

#include "Global.h"
#include "ShaderManager.h"

class CMaterial
{
public:
	DECLARE_CLASS_NOBASE( CMaterial );

	CMaterial( const char *sPath, unsigned int uiDrawFlags );
	~CMaterial();

	virtual bool Use( void );
	
	const char *GetPath( void ) const;

	bool ShouldDraw( unsigned int uiDrawFlags ) const;
	unsigned int GetDrawFlags( void ) const;

	virtual ShaderType_t GetShaderType( void ) const;

private:
	char *m_sPath;
	unsigned int m_uiDrawFlags;
};

#endif // MATERIAL_H