#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "PreGlobal.h"

typedef bool ( *FnCommandCallbackVoid_t )( void );
typedef bool ( *FnCommandCallback_t )( const std::vector<const char *> &command );

class CBaseConCommand
{
public:
	DECLARE_CLASS_NOBASE( CBaseConCommand );

	CBaseConCommand( const char *sName, const char *sHelpString );

	const char *GetName( void );

	virtual void Dispatch( const std::vector<const char *> &command );
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

	virtual void Dispatch( const std::vector<const char *> &command );

	float GetValue( void );

private:

	float m_flValue;
};

class CConInt : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConInt, CBaseConCommand );

	CConInt( const char *sName, int iDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( const std::vector<const char *> &command );

	int GetValue( void );

private:

	int m_iValue;
};

class CConBool : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConBool, CBaseConCommand );

	CConBool( const char *sName, bool bDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( const std::vector<const char *> &command );

	bool GetValue( void );

private:

	bool m_bValue;
};

class CConVec2 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConVec2, CBaseConCommand );

	CConVec2( const char *sName, const glm::vec2 &vecDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( const std::vector<const char *> &command );

	const glm::vec2 &GetValue( void );

private:

	glm::vec2 m_vecValue;
};

class CConVec3 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConVec3, CBaseConCommand );

	CConVec3( const char *sName, const glm::vec3 &vecDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( const std::vector<const char *> &command );

	const glm::vec3 &GetValue( void );

private:

	glm::vec3 m_vecValue;
};

class CConCommand : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConCommand, CBaseConCommand );

	CConCommand( const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid, const char *sHelpString = NULL );
	CConCommand( const char *sName, FnCommandCallback_t fnCommandCallback, const char *sHelpString = NULL );

	virtual void Dispatch( const std::vector<const char *> &command );

private:

	FnCommandCallbackVoid_t m_fnCommandCallbackVoid;
	FnCommandCallback_t m_fnCommandCallback;
};

class CCommandProcesser
{
public:
	DECLARE_CLASS_NOBASE( CCommandProcesser );

	static void ProcessCommand( const char *sCommand );

	static void AddCommand( CBaseConCommand *pCommand );

private:
	static void ProcessSingleCommand( char *sCommand );
	static void FixCommand( char *sCommand, std::vector<const char *> &command );

private:
	static std::vector<CBaseConCommand *> *s_pCommands;

private:
	CCommandProcesser() {}
};

#endif // COMMANDPROCESSOR_H