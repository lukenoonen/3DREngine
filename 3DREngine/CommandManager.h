#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "Global.h"
#include "TextReader.h"

typedef bool ( *FnCommandCallbackVoid_t )( void );
typedef bool ( *FnCommandCallback_t )( CTextItem *pCommand );

class CBaseConCommand
{
public:
	DECLARE_CLASS_NOBASE( CBaseConCommand );

	CBaseConCommand( const char *sName, const char *sHelpString );

	const char *GetName( void );

	virtual void Dispatch( CTextItem *pTextInformation );
	void OnDispatch( void );
	void Help( void ) const;

	bool WasDispatched( void ) const;

private:
	const char *m_sName;
	const char *m_sHelpString;

	unsigned int m_uiFrameLastDispatched;
};

class CConFloat : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConFloat, CBaseConCommand );

	CConFloat( const char *sName, float flDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( CTextItem *pTextInformation );

	float GetValue( void );

private:

	float m_flValue;
};

class CConInt : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConInt, CBaseConCommand );

	CConInt( const char *sName, int iDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( CTextItem *pTextInformation );

	int GetValue( void );

private:

	int m_iValue;
};

class CConBool : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConBool, CBaseConCommand );

	CConBool( const char *sName, bool bDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( CTextItem *pTextInformation );

	bool GetValue( void );

private:

	bool m_bValue;
};

class CConVec2 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConVec2, CBaseConCommand );

	CConVec2( const char *sName, const glm::vec2 &vecDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( CTextItem *pTextInformation );

	const glm::vec2 &GetValue( void );

private:

	glm::vec2 m_vecValue;
};

class CConVec3 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConVec3, CBaseConCommand );

	CConVec3( const char *sName, const glm::vec3 &vecDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( CTextItem *pTextInformation );

	const glm::vec3 &GetValue( void );

private:

	glm::vec3 m_vecValue;
};

class CConVec4 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConVec4, CBaseConCommand );

	CConVec4( const char *sName, const glm::vec4 &vecDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( CTextItem *pTextInformation );

	const glm::vec4 &GetValue( void );

private:

	glm::vec4 m_vecValue;
};

class CConCommand : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConCommand, CBaseConCommand );

	CConCommand( const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid, const char *sHelpString = NULL );
	CConCommand( const char *sName, FnCommandCallback_t fnCommandCallback, const char *sHelpString = NULL );

	virtual void Dispatch( CTextItem *pTextInformation );

private:

	FnCommandCallbackVoid_t m_fnCommandCallbackVoid;
	FnCommandCallback_t m_fnCommandCallback;
};

#define PREVIOUS_COMMANDS_COUNT 128

class CCommandManager
{
public:
	DECLARE_CLASS_NOBASE( CCommandManager );

	CCommandManager();
	~CCommandManager();

	static void AddCommand( CBaseConCommand *pCommand );

	void ProcessCommand( const char *sCommand );
	const char *GetPreviousCommand( unsigned int uiIndex );
	unsigned int GetPreviousCommandCount( void );
	void GetSimilarCommands( const char *sKey, std::vector<const char *> &sCommands );

private:
	static std::vector<CBaseConCommand *> *s_pCommands;

	std::vector<CBaseConCommand *> *m_pCommands;
	char *m_sPreviousCommands[PREVIOUS_COMMANDS_COUNT];
	unsigned int m_uiPreviousCommandsIndex;
	unsigned int m_uiPreviousCommandsCount;
};

#endif // COMMANDMANAGER_H