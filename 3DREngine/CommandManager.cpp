#include "CommandManager.h"
#include "TimeManager.h"
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

bool CC_Exec( const CTextLine *pCommand )
{
	const char *sFileName;
	if (!pCommand->GetValue( sFileName, 1 ))
		return false;

	if (!pFileManager->Open( sFileName, EFileType::t_config ))
		return false;

	char *sCommand;
	if (!pFileManager->Buffer( sCommand ))
		return false;

	pFileManager->Close();

	pCommandManager->ProcessCommand( sCommand );
	delete[] sCommand;
	
	return true;
}
CConCommand cc_exec( "exec", CC_Exec );

CBaseConCommand::CBaseConCommand( const char *sName )
{
	m_sName = sName;

	m_fnCommandCallback = NULL;
	m_fnCommandCallbackVoid = NULL;
	
	m_ulFrameLastDispatched = 0;

	CCommandManager::AddCommand( this );
}

CBaseConCommand::CBaseConCommand( const char *sName, FnCommandCallback_t fnCommandCallback )
{
	m_sName = sName;

	m_fnCommandCallback = fnCommandCallback;
	m_fnCommandCallbackVoid = NULL;

	m_ulFrameLastDispatched = 0;

	CCommandManager::AddCommand( this );
}

CBaseConCommand::CBaseConCommand( const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid )
{
	m_sName = sName;

	m_fnCommandCallback = NULL;
	m_fnCommandCallbackVoid = fnCommandCallbackVoid;

	m_ulFrameLastDispatched = 0;

	CCommandManager::AddCommand( this );
}

bool CBaseConCommand::Dispatch( CTextLine *pTextLine )
{
	m_ulFrameLastDispatched = pTimeManager->GetFrameCount();

	if (m_fnCommandCallbackVoid)
		return (*m_fnCommandCallbackVoid)();
	else if (m_fnCommandCallback)
		return (*m_fnCommandCallback)(pTextLine);

	return true;
}

const char *CBaseConCommand::GetName( void ) const
{
	return m_sName;
}

bool CBaseConCommand::WasDispatched( void ) const
{
	return m_ulFrameLastDispatched == pTimeManager->GetFrameCount();
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

bool CConFloat::Dispatch( CTextLine *pTextLine )
{
	float flPrevValue = m_flValue;
	if (!pTextLine->GetValue( m_flValue, 1 ))
		return false;

	if (!BaseClass::Dispatch( pTextLine ))
	{
		m_flValue = flPrevValue;
		return false;
	}

	return true;
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

bool CConInt::Dispatch( CTextLine *pTextLine )
{
	int iPrevValue = m_iValue;
	if (!pTextLine->GetValue( m_iValue, 1 ))
		return false;

	if (!BaseClass::Dispatch( pTextLine ))
	{
		m_iValue = iPrevValue;
		return false;
	}

	return true;
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

bool CConBool::Dispatch( CTextLine *pTextLine )
{
	bool bPrevValue = m_bValue;
	if (!pTextLine->GetValue( m_bValue, 1 ))
		return false;

	if (!BaseClass::Dispatch( pTextLine ))
	{
		m_bValue = bPrevValue;
		return false;
	}

	return true;
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

bool CConVec2::Dispatch( CTextLine *pTextLine )
{
	glm::vec2 vec2PrevValue = m_vec2Value;
	if (!pTextLine->GetValue( m_vec2Value, 1 ))
		return false;

	if (!BaseClass::Dispatch( pTextLine ))
	{
		m_vec2Value = vec2PrevValue;
		return false;
	}

	return true;
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

bool CConVec3::Dispatch( CTextLine *pTextLine )
{
	glm::vec3 vec3PrevValue = m_vec3Value;
	if (!pTextLine->GetValue( m_vec3Value, 1 ))
		return false;

	if (!BaseClass::Dispatch( pTextLine ))
	{
		m_vec3Value = vec3PrevValue;
		return false;
	}

	return true;
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

bool CConVec4::Dispatch( CTextLine *pTextLine )
{
	glm::vec4 vec4PrevValue = m_vec4Value;
	if (!pTextLine->GetValue( m_vec4Value, 1 ))
		return false;

	if (!BaseClass::Dispatch( pTextLine ))
	{
		m_vec4Value = vec4PrevValue;
		return false;
	}

	return true;
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

bool CConIVec2::Dispatch( CTextLine *pTextLine )
{
	glm::ivec2 vec2PrevValue = m_vec2Value;
	if (!pTextLine->GetValue( m_vec2Value, 1 ))
		return false;

	if (!BaseClass::Dispatch( pTextLine ))
	{
		m_vec2Value = vec2PrevValue;
		return false;
	}

	return true;
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

bool CConIVec3::Dispatch( CTextLine *pTextLine )
{
	glm::ivec3 vec3PrevValue = m_vec3Value;
	if (!pTextLine->GetValue( m_vec3Value, 1 ))
		return false;

	if (!BaseClass::Dispatch( pTextLine ))
	{
		m_vec3Value = vec3PrevValue;
		return false;
	}

	return true;
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

bool CConIVec4::Dispatch( CTextLine *pTextLine )
{
	glm::ivec4 vec4PrevValue = m_vec4Value;
	if (!pTextLine->GetValue( m_vec4Value, 1 ))
		return false;

	if (!BaseClass::Dispatch( pTextLine ))
	{
		m_vec4Value = vec4PrevValue;
		return false;
	}

	return true;
}

const glm::ivec4 &CConIVec4::GetValue( void )
{
	return m_vec4Value;
}

CConString::CConString( const char *sDefaultValue, const char *sName ) : BaseClass( sName )
{
	m_sValue = UTIL_strdup( sDefaultValue );
}

CConString::CConString( const char *sDefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback ) : BaseClass( sName, fnCommandCallback )
{
	m_sValue = UTIL_strdup( sDefaultValue );
}

CConString::CConString( const char *sDefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid ) : BaseClass( sName, fnCommandCallbackVoid )
{
	m_sValue = UTIL_strdup( sDefaultValue );
}

CConString::~CConString()
{
	delete[] m_sValue;
}

bool CConString::Dispatch( CTextLine *pTextLine )
{
	char *sPrevValue = m_sValue;
	if (!pTextLine->GetValue( m_sValue, 1 ))
		return false;

	if (!BaseClass::Dispatch( pTextLine ))
	{
		delete[] m_sValue;
		m_sValue = sPrevValue;
		return false;
	}

	return true;
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
}

CCommandManager::~CCommandManager()
{
	delete m_pCommands;
}

void CCommandManager::AddCommand( CBaseConCommand *pCommand )
{
	if (!s_pCommands)
		s_pCommands = new std::vector<CBaseConCommand *>();

	s_pCommands->push_back( pCommand );
}

void CCommandManager::ProcessCommand( const char *sCommand )
{
	if (!m_pCommands)
		return;

	CTextReader trCommand;
	if (!trCommand.ReadText( sCommand ))
		return; // What to do upon failure?

	CTextBlock *pTextBlock = trCommand.GetTextBlock();

	for (unsigned int i = 0; i < pTextBlock->GetTextLineCount(); i++)
	{
		CTextLine *pTextLine = pTextBlock->GetTextLine( i );
		for (unsigned int j = 0; j < m_pCommands->size(); j++)
		{
			CBaseConCommand *pBaseConCommand = (*m_pCommands)[j];
			if (pTextLine->IsKey( pBaseConCommand->GetName() ))
			{
				pBaseConCommand->Dispatch( pTextLine );
				break;
			}
		}
	}
}

void CCommandManager::GetSimilarCommands( const char *sKey, std::vector<const char *> &sCommands )
{
	for (unsigned int i = 0; i < m_pCommands->size(); i++)
	{
		const char *sName = (*m_pCommands)[i]->GetName();
		if (UTIL_strstr( sName, sKey ))
			sCommands.push_back( sName );
	}
}