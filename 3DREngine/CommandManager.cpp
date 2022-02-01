#include "CommandManager.h"
#include "GlobalManager.h"
#include "FileManager.h"

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
CConCommand cc_exec( "exec", CC_Exec );

CBaseConCommand::CBaseConCommand( const char *sName )
{
	m_sName = sName;

	m_fnCommandCallback = NULL;
	m_fnCommandCallbackVoid = NULL;
	
	m_uiFrameLastDispatched = 0;

	CCommandManager::AddCommand( this );
}

CBaseConCommand::CBaseConCommand( const char *sName, FnCommandCallback_t fnCommandCallback )
{
	m_sName = sName;

	m_fnCommandCallback = fnCommandCallback;
	m_fnCommandCallbackVoid = NULL;

	m_uiFrameLastDispatched = 0;

	CCommandManager::AddCommand( this );
}

CBaseConCommand::CBaseConCommand( const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid )
{
	m_sName = sName;

	m_fnCommandCallback = NULL;
	m_fnCommandCallbackVoid = fnCommandCallbackVoid;

	m_uiFrameLastDispatched = 0;

	CCommandManager::AddCommand( this );
}

bool CBaseConCommand::Dispatch( CTextItem *pTextInformation )
{
	m_uiFrameLastDispatched = pGlobalValues->GetFrameCount();

	if (m_fnCommandCallbackVoid)
		return (*m_fnCommandCallbackVoid)();
	else if (m_fnCommandCallback)
		return (*m_fnCommandCallback)(pTextInformation);

	return true;
}

const char *CBaseConCommand::GetName( void )
{
	return m_sName;
}

bool CBaseConCommand::WasDispatched( void ) const
{
	return m_uiFrameLastDispatched == pGlobalValues->GetFrameCount();
}

CConFloat::CConFloat( float flDefaultValue, const char *sName ) : BaseClass( sName )
{
	m_flValue = flDefaultValue;
}

CConFloat::CConFloat( float flDefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_flValue = flDefaultValue;
}

CConFloat::CConFloat( float flDefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_flValue = flDefaultValue;
}

bool CConFloat::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsFloatFormat())
	{
		float flPrevValue = m_flValue;
		m_flValue = pTextTerm->GetFloat();
		if (BaseClass::Dispatch( pTextInformation ))
			return true;

		m_flValue = flPrevValue;
	}

	return false;
}

float CConFloat::GetValue( void )
{
	return m_flValue;
}

CConInt::CConInt( int iDefaultValue, const char *sName ) : BaseClass( sName )
{
	m_iValue = iDefaultValue;
}

CConInt::CConInt( int iDefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_iValue = iDefaultValue;
}

CConInt::CConInt( int iDefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_iValue = iDefaultValue;
}

bool CConInt::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsIntFormat())
	{
		int iPrevValue = m_iValue;
		m_iValue = pTextTerm->GetInt();
		if (BaseClass::Dispatch( pTextInformation ))
			return true;

		m_iValue = iPrevValue;
	}

	return false;
}

int CConInt::GetValue( void )
{
	return m_iValue;
}

CConBool::CConBool( bool bDefaultValue, const char *sName ) : BaseClass( sName )
{
	m_bValue = bDefaultValue;
}

CConBool::CConBool( bool bDefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_bValue = bDefaultValue;
}

CConBool::CConBool( bool bDefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_bValue = bDefaultValue;
}

bool CConBool::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsBoolFormat())
	{
		bool bPrevValue = m_bValue;
		m_bValue = pTextTerm->GetBool();
		if (BaseClass::Dispatch( pTextInformation ))
			return true;

		m_bValue = bPrevValue;
	}

	return false;
}

bool CConBool::GetValue( void )
{
	return m_bValue;
}

CConVec2::CConVec2( const glm::vec2 &vec2DefaultValue, const char *sName ) : BaseClass( sName )
{
	m_vec2Value = vec2DefaultValue;
}

CConVec2::CConVec2( const glm::vec2 &vec2DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_vec2Value = vec2DefaultValue;
}

CConVec2::CConVec2( const glm::vec2 &vec2DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_vec2Value = vec2DefaultValue;
}

bool CConVec2::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsVec2Format())
	{
		glm::vec2 vec2PrevValue = m_vec2Value;
		m_vec2Value = pTextTerm->GetVec2();
		if (BaseClass::Dispatch( pTextInformation ))
			return true;

		m_vec2Value = vec2PrevValue;
	}

	return false;
}

const glm::vec2 &CConVec2::GetValue( void )
{
	return m_vec2Value;
}

CConVec3::CConVec3( const glm::vec3 &vec3DefaultValue, const char *sName ) : BaseClass( sName )
{
	m_vec3Value = vec3DefaultValue;
}

CConVec3::CConVec3( const glm::vec3 &vec3DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_vec3Value = vec3DefaultValue;
}

CConVec3::CConVec3( const glm::vec3 &vec3DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_vec3Value = vec3DefaultValue;
}

bool CConVec3::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsVec3Format())
	{
		glm::vec3 vec3PrevValue = m_vec3Value;
		m_vec3Value = pTextTerm->GetVec3();
		if (BaseClass::Dispatch( pTextInformation ))
			return true;

		m_vec3Value = vec3PrevValue;
	}

	return false;
}

const glm::vec3 &CConVec3::GetValue( void )
{
	return m_vec3Value;
}

CConVec4::CConVec4( const glm::vec4 &vec4DefaultValue, const char *sName ) : BaseClass( sName )
{
	m_vec4Value = vec4DefaultValue;
}

CConVec4::CConVec4( const glm::vec4 &vec4DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_vec4Value = vec4DefaultValue;
}

CConVec4::CConVec4( const glm::vec4 &vec4DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_vec4Value = vec4DefaultValue;
}

bool CConVec4::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsVec4Format())
	{
		glm::vec4 vec4PrevValue = m_vec4Value;
		m_vec4Value = pTextTerm->GetVec4();
		if (BaseClass::Dispatch( pTextInformation ))
			return true;

		m_vec4Value = vec4PrevValue;
	}

	return false;
}

const glm::vec4 &CConVec4::GetValue( void )
{
	return m_vec4Value;
}

CConIVec2::CConIVec2( const glm::ivec2 &vec2DefaultValue, const char *sName ) : BaseClass( sName )
{
	m_vec2Value = vec2DefaultValue;
}

CConIVec2::CConIVec2( const glm::ivec2 &vec2DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_vec2Value = vec2DefaultValue;
}

CConIVec2::CConIVec2( const glm::ivec2 &vec2DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_vec2Value = vec2DefaultValue;
}

bool CConIVec2::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsIVec2Format())
	{
		glm::ivec2 vec2PrevValue = m_vec2Value;
		m_vec2Value = pTextTerm->GetIVec2();
		if (BaseClass::Dispatch( pTextInformation ))
			return true;

		m_vec2Value = vec2PrevValue;
	}

	return false;
}

const glm::ivec2 &CConIVec2::GetValue( void )
{
	return m_vec2Value;
}

CConIVec3::CConIVec3( const glm::ivec3 &vec3DefaultValue, const char *sName ) : BaseClass( sName )
{
	m_vec3Value = vec3DefaultValue;
}

CConIVec3::CConIVec3( const glm::ivec3 &vec3DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_vec3Value = vec3DefaultValue;
}

CConIVec3::CConIVec3( const glm::ivec3 &vec3DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_vec3Value = vec3DefaultValue;
}

bool CConIVec3::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsIVec3Format())
	{
		glm::ivec3 vec3PrevValue = m_vec3Value;
		m_vec3Value = pTextTerm->GetIVec3();
		if (BaseClass::Dispatch( pTextInformation ))
			return true;

		m_vec3Value = vec3PrevValue;
	}

	return false;
}

const glm::ivec3 &CConIVec3::GetValue( void )
{
	return m_vec3Value;
}

CConIVec4::CConIVec4( const glm::ivec4 &vec4DefaultValue, const char *sName ) : BaseClass( sName )
{
	m_vec4Value = vec4DefaultValue;
}

CConIVec4::CConIVec4( const glm::ivec4 &vec4DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_vec4Value = vec4DefaultValue;
}

CConIVec4::CConIVec4( const glm::ivec4 &vec4DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_vec4Value = vec4DefaultValue;
}

bool CConIVec4::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm && pTextTerm->IsIVec4Format())
	{
		glm::ivec4 vec4PrevValue = m_vec4Value;
		m_vec4Value = pTextTerm->GetIVec4();
		if (BaseClass::Dispatch( pTextInformation ))
			return true;

		m_vec4Value = vec4PrevValue;
	}

	return false;
}

const glm::ivec4 &CConIVec4::GetValue( void )
{
	return m_vec4Value;
}

CConString::CConString( const char *sDefaultValue, const char *sName ) : BaseClass( sName )
{
	m_sValue = UTIL_stredit( sDefaultValue );
}

CConString::CConString( const char *sDefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_sValue = UTIL_stredit( sDefaultValue );
}

CConString::CConString( const char *sDefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_sValue = UTIL_stredit( sDefaultValue );
}

CConString::~CConString()
{
	delete[] m_sValue;
}

bool CConString::Dispatch( CTextItem *pTextInformation )
{
	CTextTerm *pTextTerm = pTextInformation->GetTextTerm( 1 );
	if (pTextTerm)
	{
		char *sPrevValue = m_sValue;
		m_sValue = UTIL_stredit( pTextTerm->GetString() );
		if (BaseClass::Dispatch( pTextInformation ))
			return true;

		delete[] m_sValue;
		m_sValue = sPrevValue;
	}

	return false;
}

const char *CConString::GetValue( void )
{
	return m_sValue;
}

CConCommand::CConCommand(const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{

}

CConCommand::CConCommand( const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{

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
		return;

	CTextReader trCommand( sCommand );
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