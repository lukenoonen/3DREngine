#include "GUILog.h"
#include "KeyValues.h"
#include "EntityManager.h"

DEFINE_DATADESC( CGUILog )

	DEFINE_FIELD( IterableDataField, std::vector<char *>, m_sLogs, "logs", FL_NONE )

	DEFINE_FIELD( LinkedDataField, CHandle<CTextMaterial>, m_hTextMaterial, "textmaterial", FL_REQUIRED )

	DEFINE_FIELD( DataField, glm::vec2, m_vec2Bounds, "bounds", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flLineScale, "linescale", FL_NONE )
	DEFINE_FIELD( DataField, ETextAlign, m_eTextAlign, "textalign", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( CGUILog, log_gui )

CGUILog::CGUILog()
{
	m_vec2Bounds = g_vec2One;
	m_flLineScale = 1.0f;
	m_eTextAlign = ETextAlign::t_left;
	m_flOffset = 0.0f;
}

bool CGUILog::Init( void )
{
	if (!BaseClass::Init())
		return false;

	for (unsigned int i = 0; i < m_sLogs.size(); i++)
	{
		char *sLog = m_sLogs[i];
		AddText( sLog );
		delete sLog;
	}
	m_sLogs.clear();
	
	return true;
}

void CGUILog::Remove( void ) // TODO: finish work on remove
{
	for (unsigned int i = 0; i < m_hGUITexts.size(); i++)
	{
		m_hGUITexts[i]->Remove();
		m_hGUITexts[i].Check();
	}

	BaseClass::Remove();
}

void CGUILog::AddText( const char *sText )
{
	CKeyValues kvTextInit;
	kvTextInit.Insert( "name", UTIL_strdup( sText ) );
	kvTextInit.Insert( "text", UTIL_strdup( sText ) );
	kvTextInit.Insert( "textmaterial", m_hTextMaterial );
	kvTextInit.Insert( "bounds", m_vec2Bounds );
	kvTextInit.Insert( "linescale", m_flLineScale );
	kvTextInit.Insert( "textalign", m_eTextAlign );
	kvTextInit.Insert( "drawflags", GetDrawFlags() );
	kvTextInit.Insert( "flags", GetFlags() );

	CGUIText *pGUIText = pEntityManager->AddEntity<CGUIText>( &kvTextInit, GetLoadGroup() );
	if (pGUIText)
	{
		pEntityManager->FlushLoadGroup( GetLoadGroup() );

		float flSize = pGUIText->GetPointSize() * (float)pGUIText->GetLineCount();
		AddPosition( g_vec3Up * flSize );
		pGUIText->SetPosition( g_vec3Down * (m_flOffset + pGUIText->GetPointSize()) );
		pGUIText->SetRotation( g_vec3Zero );
		pGUIText->SetScale( g_vec3One );
		pGUIText->SetParentRelative( this );
		m_flOffset += flSize;
		
		m_hGUITexts.emplace_back( pGUIText );
	}
}