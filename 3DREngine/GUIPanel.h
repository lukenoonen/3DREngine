#ifndef GUIPANEL_H
#define GUIPANEL_H

#include "Global.h"
#include "BaseWorld2D.h"
#include "GUIPanelMaterial.h"

class CGUIPanel : public CBaseWorld2D
{
public:
	DECLARE_CLASS( CGUIPanel, CBaseWorld2D )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CGUIPanel();
	virtual ~CGUIPanel();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Draw( void );
	virtual bool ShouldDraw( void ) const;

private:
	GLuint m_glVAO;
	GLuint m_glVBO;

	CHandle<CGUIPanelMaterial> m_hGUIPanelMaterial;
};

#endif // GUIPANEL_H