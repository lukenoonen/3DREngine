#include "CommandProcessor.h"
#include "GlobalManager.h"

CBaseConCommand::CBaseConCommand( const char *sName, const char *sHelpString )
{
	m_sName = sName;
	m_sHelpString = sHelpString;
}

const char *CBaseConCommand::GetName( void )
{
	return m_sName;
}

void CBaseConCommand::Dispatch( const std::vector<const char *> &command )
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

	CCommandProcesser::AddCommand( this );
}

void CConFloat::Dispatch( const std::vector<const char *> &command )
{
	if (command.size() > 0)
		m_flValue = UTIL_atof( command[0] );
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

	CCommandProcesser::AddCommand( this );
}

void CConInt::Dispatch( const std::vector<const char *> &command )
{
	if (command.size() > 0)
		m_iValue = UTIL_atoi( command[0] );
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

	CCommandProcesser::AddCommand( this );
}

void CConBool::Dispatch( const std::vector<const char *> &command )
{
	if (command.size() > 0)
		m_bValue = UTIL_atoi( command[0] ) != 0;
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

	CCommandProcesser::AddCommand( this );
}

void CConVec2::Dispatch( const std::vector<const char *> &command )
{
	if (command.size() > 1)
	{
		m_vecValue.x = UTIL_atof( command[0] );
		m_vecValue.y = UTIL_atof( command[1] );
	}
	else
	{
		Help();
	}

	OnDispatch();
}

const glm::vec2 &CConVec2::GetValue( void )
{
	return m_vecValue;
}

CConVec3::CConVec3( const char *sName, const glm::vec3 &bDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_vecValue = bDefaultValue;

	CCommandProcesser::AddCommand( this );
}

void CConVec3::Dispatch( const std::vector<const char *> &command )
{
	if (command.size() > 2)
	{
		m_vecValue.x = UTIL_atof( command[0] );
		m_vecValue.y = UTIL_atof( command[1] );
		m_vecValue.z = UTIL_atof( command[2] );
	}
	else
	{
		Help();
	}

	OnDispatch();
}

const glm::vec3 &CConVec3::GetValue( void )
{
	return m_vecValue;
}

CConCommand::CConCommand(const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid, const char *sHelpString /*= NULL*/) : BaseClass( sName, sHelpString )
{
	m_fnCommandCallbackVoid = fnCommandCallbackVoid;
	m_fnCommandCallback = NULL;
	
	CCommandProcesser::AddCommand( this );
}

CConCommand::CConCommand( const char *sName, FnCommandCallback_t fnCommandCallback, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_fnCommandCallbackVoid = NULL;
	m_fnCommandCallback = fnCommandCallback;

	CCommandProcesser::AddCommand( this );
}

void CConCommand::Dispatch( const std::vector<const char *> &command )
{
	if (m_fnCommandCallbackVoid)
	{
		if (!(*m_fnCommandCallbackVoid)())
			Help();
	}
	else if (m_fnCommandCallback)
	{
		if (!(*m_fnCommandCallback)( command ))
			Help();
	}

	OnDispatch();
}

std::vector<CBaseConCommand *> *CCommandProcesser::s_pCommands = NULL;

void CCommandProcesser::ProcessCommand( const char *sCommand )
{
	char *sCommandEditable = UTIL_stredit( sCommand );

	char *sSearch = sCommandEditable;
	char *sChar = sCommandEditable;

	bool bSkip = false;

	while (true)
	{
		sChar = UTIL_strchr( sChar, "\";\n" );

		if (!sChar)
			break;

		if (*sChar == '\"')
		{
			bSkip = !bSkip;
			sChar++;
		}
		else
		{
			if (!bSkip)
			{
				*sChar++ = '\0';

				if (*sSearch)
					ProcessSingleCommand( sSearch );

				sSearch = sChar;
			}
			else
			{
				sChar++;
			}
		}
	}
	
	if (*sSearch)
		ProcessSingleCommand( sSearch );

	delete[] sCommandEditable;
}

void CCommandProcesser::AddCommand( CBaseConCommand *pCommand )
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

void CCommandProcesser::ProcessSingleCommand( char *sCommand )
{
	if (!s_pCommands)
	{
		// pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_red, "Warning: no console commands!" );
		return;
	}

	std::vector<const char *> command;
	FixCommand( sCommand, command );

	for (unsigned int i = 0; i < s_pCommands->size(); i++)
	{
		CBaseConCommand *pBaseConCommand = (*s_pCommands)[i];

		if (UTIL_strcmp( pBaseConCommand->GetName(), command[0] ) == 0)
		{
			command.erase( command.begin() );
			pBaseConCommand->Dispatch( command );
			return;
		}
	}

	// pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_red, "Warning: unknown command '%s'", command[0] );
}

void CCommandProcesser::FixCommand( char *sCommand, std::vector<const char *> &command )
{
	const char *sToAdd;
	bool bPreviousWasQuote = false;
	while (true)
	{
		char *sChar;
		if (bPreviousWasQuote)
			sChar = UTIL_strchr( sCommand, '\"' );
		else
			sChar = UTIL_strchr( sCommand, " \"" );

		if (!sChar)
			break;

		if (*sChar == '\"' && !bPreviousWasQuote)
			bPreviousWasQuote = true;
		else
			bPreviousWasQuote = false;

		*sChar++ = '\0';

		if (*sCommand)
		{
			sToAdd = sCommand;
			command.push_back( sToAdd );
		}

		sCommand = sChar;
	}

	if (*sCommand)
	{
		sToAdd = sCommand;
		command.push_back( sToAdd );
	}
}