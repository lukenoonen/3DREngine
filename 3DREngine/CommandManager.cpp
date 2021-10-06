#include "CommandManager.h"
#include "GlobalManager.h"
#include "FileManager.h"

bool CC_Exec( CTextItem *pCommand )
{
	if (pCommand->GetTextTermCount() < 2)
		return false;

	char *sConfigContents;
	if (!pFileManager->ReadEntireFile( pCommand->GetTextTerm( 1 )->GetString(), sConfigContents ))
		return false;

	pCommandManager->ProcessCommand( sConfigContents );
	return true;
}
CConCommand cc_exec( "exec", CC_Exec, "exec (file name)" );

/*if (command.size() == 0)
{
	char sFilename[MAX_PATH];
	ZeroMemory( &sFilename, sizeof( sFilename ) );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( ofn ) );
	ofn.lStructSize = sizeof( ofn );
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "\0Config Files\0*.cfg";
	ofn.lpstrFile = sFilename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a File";
	ofn.lpstrDefExt = ".sit";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA( &ofn ))
		pFileManager->Load( sFilename );
}
else
{
	pFileManager->Load( command[0] );
}*/

CBaseConCommand::CBaseConCommand( const char *sName, const char *sHelpString )
{
	m_sName = sName;
	m_sHelpString = sHelpString;
}

const char *CBaseConCommand::GetName( void )
{
	return m_sName;
}

void CBaseConCommand::Dispatch( CTextItem *pTextInformation )
{

}

void CBaseConCommand::OnDispatch( void )
{
	m_uiFrameLastDispatched = pGlobalValues->GetFrameCount();
}

void CBaseConCommand::Help( void ) const
{
	//if (m_sHelpString)
	//	pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_red, "'%s' Usage: %s", m_sName, m_sHelpString );
}

bool CBaseConCommand::WasDispatched( void ) const
{
	return m_uiFrameLastDispatched == pGlobalValues->GetFrameCount();
}

CConFloat::CConFloat( const char *sName, float flDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_flValue = flDefaultValue;

	CCommandManager::AddCommand( this );
}

void CConFloat::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsFloatFormat())
		m_flValue = pTextTerm->GetFloat();
	else
		Help();

	OnDispatch();
}

float CConFloat::GetValue( void )
{
	return m_flValue;
}

CConInt::CConInt( const char *sName, int iDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_iValue = iDefaultValue;

	CCommandManager::AddCommand( this );
}

void CConInt::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsIntFormat())
		m_iValue = pTextTerm->GetInt();
	else
		Help();

	OnDispatch();
}

int CConInt::GetValue( void )
{
	return m_iValue;
}

CConBool::CConBool( const char *sName, bool bDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_bValue = bDefaultValue;

	CCommandManager::AddCommand( this );
}

void CConBool::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsBoolFormat())
		m_bValue = pTextTerm->GetBool();
	else
		Help();

	OnDispatch();
}

bool CConBool::GetValue( void )
{
	return m_bValue;
}

CConVec2::CConVec2( const char *sName, const glm::vec2 &bDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_vecValue = bDefaultValue;

	CCommandManager::AddCommand( this );
}

void CConVec2::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsVec2Format())
		m_vecValue = pTextTerm->GetVec2();
	else
		Help();

	OnDispatch();
}

const glm::vec2 &CConVec2::GetValue( void )
{
	return m_vecValue;
}

CConVec3::CConVec3( const char *sName, const glm::vec3 &bDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_vecValue = bDefaultValue;

	CCommandManager::AddCommand( this );
}

void CConVec3::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsVec3Format())
		m_vecValue = pTextTerm->GetVec3();
	else
		Help();

	OnDispatch();
}

const glm::vec3 &CConVec3::GetValue( void )
{
	return m_vecValue;
}

CConVec4::CConVec4( const char *sName, const glm::vec4 &bDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_vecValue = bDefaultValue;

	CCommandManager::AddCommand( this );
}

void CConVec4::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsVec4Format())
		m_vecValue = pTextTerm->GetVec4();
	else
		Help();

	OnDispatch();
}

const glm::vec4 &CConVec4::GetValue( void )
{
	return m_vecValue;
}

CConCommand::CConCommand(const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid, const char *sHelpString /*= NULL*/) : BaseClass( sName, sHelpString )
{
	m_fnCommandCallbackVoid = fnCommandCallbackVoid;
	m_fnCommandCallback = NULL;
	
	CCommandManager::AddCommand( this );
}

CConCommand::CConCommand( const char *sName, FnCommandCallback_t fnCommandCallback, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_fnCommandCallbackVoid = NULL;
	m_fnCommandCallback = fnCommandCallback;

	CCommandManager::AddCommand( this );
}

void CConCommand::Dispatch( CTextItem *pTextInformation )
{
	if (m_fnCommandCallbackVoid)
	{
		if (!(*m_fnCommandCallbackVoid)())
			Help();
	}
	else if (m_fnCommandCallback)
	{
		if (!(*m_fnCommandCallback)(pTextInformation))
			Help();
	}

	OnDispatch();
}

std::vector<CBaseConCommand *> *CCommandManager::s_pCommands = NULL;

CCommandManager::CCommandManager()
{
	m_pCommands = s_pCommands;
	s_pCommands = NULL;

	for (unsigned int i = 0; i < PREVIOUS_COMMANDS_COUNT; i++)
		m_sPreviousCommands[i] = NULL;

	m_uiPreviousCommandsIndex = 0;
	m_uiPreviousCommandsCount = 0;
}

CCommandManager::~CCommandManager()
{
	delete m_pCommands;

	for (unsigned int i = 0; i < PREVIOUS_COMMANDS_COUNT; i++)
	{
		if (m_sPreviousCommands[i])
			delete[] m_sPreviousCommands[i];
	}
}

void CCommandManager::AddCommand( CBaseConCommand *pCommand )
{
	if (!s_pCommands)
		s_pCommands = new std::vector<CBaseConCommand *>();

	for (unsigned int i = 0; i < s_pCommands->size(); i++)
	{
		if ((*s_pCommands)[i]->GetName() == pCommand->GetName())
			return;
	}

	s_pCommands->push_back( pCommand );
}

void CCommandManager::ProcessCommand( const char *sCommand )
{
	if (m_sPreviousCommands[m_uiPreviousCommandsIndex])
		delete[] m_sPreviousCommands[m_uiPreviousCommandsIndex];

	m_sPreviousCommands[m_uiPreviousCommandsIndex] = UTIL_stredit( sCommand );
	m_uiPreviousCommandsIndex = (m_uiPreviousCommandsIndex + 1) % PREVIOUS_COMMANDS_COUNT;

	if (m_uiPreviousCommandsCount < PREVIOUS_COMMANDS_COUNT)
		m_uiPreviousCommandsCount++;

	if (!m_pCommands)
	{
		// pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_red, "Warning: no console commands!" );
		return;
	}

	CTextReader trCommand;
	trCommand.InitFromText( sCommand );
	CTextInformation *pTextInformation = trCommand.GetTextInformation();

	for (unsigned int i = 0; i < pTextInformation->GetTextItemCount(); i++)
	{
		CTextItem *pTextItem = pTextInformation->GetTextItem( i );
		for (unsigned int j = 0; j < m_pCommands->size(); j++)
		{
			CBaseConCommand *pBaseConCommand = (*m_pCommands)[j];

			if (UTIL_streq( pBaseConCommand->GetName(), pTextItem->GetKey() ))
			{
				pBaseConCommand->Dispatch( pTextItem );
				break;
			}
		}
	}
}

const char *CCommandManager::GetPreviousCommand( unsigned int uiIndex )
{
	if (uiIndex >= PREVIOUS_COMMANDS_COUNT)
		return NULL;

	if (uiIndex > m_uiPreviousCommandsIndex)
		uiIndex = PREVIOUS_COMMANDS_COUNT + m_uiPreviousCommandsIndex - uiIndex;
	else
		uiIndex = m_uiPreviousCommandsIndex - uiIndex;

	return m_sPreviousCommands[uiIndex];
}

unsigned int CCommandManager::GetPreviousCommandCount( void )
{
	return m_uiPreviousCommandsCount;
}

void CCommandManager::GetSimilarCommands( const char *sKey, std::vector<const char *> &sCommands )
{
	for (unsigned int i = 0; i < m_pCommands->size(); i++)
	{
		const char *sName = (*m_pCommands)[i]->GetName();
		if (UTIL_strinc( sName, sKey ))
			sCommands.push_back( sName );
	}
}