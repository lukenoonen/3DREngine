#ifndef FLATTEXTURE_H
#define FLATTEXTURE_H

#include "Global.h"
#include "BaseTexture.h"

enum class ETextureWrap : EBaseEnum
{
	t_repeat = 0,
	t_mirrored,
	t_edge,
	t_border,

	i_count,
	i_invalid = i_count,
};

class CFlatTexture : public CBaseTexture
{
public:
	DECLARE_CLASS( CFlatTexture, CBaseTexture )

	CFlatTexture();

	virtual bool Init( void );
	virtual bool Exit( void );

	virtual int Bind( void );

	void SetTextureWrap( ETextureWrap eTextureWrap );
	void SetBorderColor( const glm::vec4 &vec4BorderColor );

private:
	GLuint m_glID; // TODO: consider moving this to BaseTexture

	ETextureWrap m_eTextureWrap;
	glm::vec4 m_vec4BorderColor;
};

#endif // FLATTEXTURE_H