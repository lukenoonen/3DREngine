#ifndef GUILOG_H
#define GUILOG_H

#include "Global.h"
#include "BaseBillboard.h"
#include "GUIText.h"

class CGUILog : public CBaseBillboard
{
public:
	DECLARE_CLASS( CGUILog, CBaseBillboard )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CGUILog();

	virtual bool Init( void );

	virtual void Remove( void );

	void AddText( const char *sText );

private:
	std::vector<CHandle<CGUIText>> m_hGUITexts;

	std::vector<char *> m_sLogs;

	CHandle<CTextMaterial> m_hTextMaterial;

	glm::vec2 m_vec2Bounds;
	float m_flLineScale;
	ETextAlign m_eTextAlign;

	float m_flOffset;
};

#endif // GUILOG_H