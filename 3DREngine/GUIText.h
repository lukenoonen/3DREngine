#ifndef GUITEXT_H
#define GUITEXT_H

#include "Global.h"
#include "BaseWorld2D.h"
#include "TextMaterial.h"

enum class ETextAlign : EBaseEnum
{
	t_left = 0,
	t_center,
	t_right,
	t_block,

	i_count,
	i_invalid = i_count,
};

static const char *g_sETextAlignNames[] =
{
	"left",
	"center",
	"right",
	"block",
};

DEFINE_ENUM_NAMES( ETextAlign, g_sETextAlignNames )

DEFINE_ENUM_GETVALUE( ETextAlign )

struct SGUITextChar
{
	char cChar;
	float flXPos;
};

struct SVertex2D
{
	glm::vec2 vec2Position;
	glm::vec2 vec2TexCoords;
};

class CGUIText : public CBaseWorld2D
{
public:
	DECLARE_CLASS( CGUIText, CBaseWorld2D )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CGUIText();
	virtual ~CGUIText();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Draw( void );
	virtual bool ShouldDraw( void ) const;

	void SetText( const char *sText );

private:
	void RecalculateText( void );
	void CalculateText( void );
	void DeleteText( void );

	virtual const glm::vec2 &GetAlignFactor( void ) const;

private:
	char *m_sText;

	std::vector<SVertex2D> m_verVertices;

	GLuint m_glVAO;
	GLuint m_glVBO;

	CHandle<CTextMaterial> m_hTextMaterial;

	glm::vec2 m_vec2Bounds;
	float m_flLineScale;
	ETextAlign m_eTextAlign;

};

#endif // GUITEXT_H