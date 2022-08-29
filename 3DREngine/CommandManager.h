#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "Global.h"
#include "TextReader.h"

typedef bool ( *FnCommandCallbackVoid_t )( void );
typedef bool ( *FnCommandCallback_t )( CTextLine *pCommand );

// TODO: consider templatising these

class CBaseConCommand
{
public:
	DECLARE_CLASS_NOBASE( CBaseConCommand )

	CBaseConCommand( const char *sName );
	CBaseConCommand( const char *sName, FnCommandCallback_t fnCommandCallback );
	CBaseConCommand( const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );

	virtual bool Dispatch( CTextLine *pTextLine );

	const char *GetName( void );

	bool WasDispatched( void ) const;

private:
	const char *m_sName;

	FnCommandCallback_t m_fnCommandCallback;
	FnCommandCallbackVoid_t m_fnCommandCallbackVoid;

	unsigned long m_ulFrameLastDispatched;
};

class CConFloat : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConFloat, CBaseConCommand )

	CConFloat( float flDefaultValue, const char *sName );
	CConFloat( float flDefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback );
	CConFloat( float flDefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );

	virtual bool Dispatch( CTextLine *pTextLine );

	float GetValue( void );

private:
	float m_flValue;
};

class CConInt : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConInt, CBaseConCommand )

	CConInt( int iDefaultValue, const char *sName );
	CConInt( int iDefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback );
	CConInt( int iDefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );

	virtual bool Dispatch( CTextLine *pTextLine );

	int GetValue( void );

private:
	int m_iValue;
};

class CConBool : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConBool, CBaseConCommand )

	CConBool( bool bDefaultValue, const char *sName );
	CConBool( bool bDefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback );
	CConBool( bool bDefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );

	virtual bool Dispatch( CTextLine *pTextLine );

	bool GetValue( void );

private:
	bool m_bValue;
};

class CConVec2 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConVec2, CBaseConCommand )

	CConVec2( const glm::vec2 &vec2DefaultValue, const char *sName );
	CConVec2( const glm::vec2 &vec2DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback );
	CConVec2( const glm::vec2 &vec2DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );

	virtual bool Dispatch( CTextLine *pTextLine );

	const glm::vec2 &GetValue( void );

private:
	glm::vec2 m_vec2Value;
};

class CConVec3 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConVec3, CBaseConCommand )

	CConVec3( const glm::vec3 &vec3DefaultValue, const char *sName );
	CConVec3( const glm::vec3 &vec3DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback );
	CConVec3( const glm::vec3 &vec3DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );

	virtual bool Dispatch( CTextLine *pTextLine );

	const glm::vec3 &GetValue( void );

private:
	glm::vec3 m_vec3Value;
};

class CConVec4 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConVec4, CBaseConCommand )

	CConVec4( const glm::vec4 &vec4DefaultValue, const char *sName );
	CConVec4( const glm::vec4 &vec4DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback );
	CConVec4( const glm::vec4 &vec4DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );

	virtual bool Dispatch( CTextLine *pTextLine );

	const glm::vec4 &GetValue( void );

private:
	glm::vec4 m_vec4Value;
};

class CConIVec2 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConIVec2, CBaseConCommand )
	
	CConIVec2( const glm::ivec2 &vec2DefaultValue, const char *sName );
	CConIVec2( const glm::ivec2 &vec2DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback );
	CConIVec2( const glm::ivec2 &vec2DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );

	virtual bool Dispatch( CTextLine *pTextLine );

	const glm::ivec2 &GetValue( void );

private:
	glm::ivec2 m_vec2Value;
};

class CConIVec3 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConIVec3, CBaseConCommand )

	CConIVec3( const glm::ivec3 &vec3DefaultValue, const char *sName );
	CConIVec3( const glm::ivec3 &vec3DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback );
	CConIVec3( const glm::ivec3 &vec3DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );

	virtual bool Dispatch( CTextLine *pTextLine );

	const glm::ivec3 &GetValue( void );

private:
	glm::ivec3 m_vec3Value;
};

class CConIVec4 : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConIVec4, CBaseConCommand )

	CConIVec4( const glm::ivec4 &vec4DefaultValue, const char *sName );
	CConIVec4( const glm::ivec4 &vec4DefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback );
	CConIVec4( const glm::ivec4 &vec4DefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );

	virtual bool Dispatch( CTextLine *pTextLine );

	const glm::ivec4 &GetValue( void );

private:
	glm::ivec4 m_vec4Value;
};

class CConString : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConString, CBaseConCommand )

	CConString( const char *sDefaultValue, const char *sName );
	CConString( const char *sDefaultValue, const char *sName, FnCommandCallback_t fnCommandCallback );
	CConString( const char *sDefaultValue, const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );
	~CConString();

	virtual bool Dispatch( CTextLine *pTextLine );

	const char *GetValue( void );

private:
	char *m_sValue;
};

class CConCommand : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConCommand, CBaseConCommand )

	CConCommand( const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid );
	CConCommand( const char *sName, FnCommandCallback_t fnCommandCallback );
};

class CCommandManager
{
public:
	DECLARE_CLASS_NOBASE( CCommandManager )

	CCommandManager();
	~CCommandManager();

	static void AddCommand( CBaseConCommand *pCommand ); // TODO: Consider cleaning this up

	void ProcessCommand( const char *sCommand );
	void GetSimilarCommands( const char *sKey, std::vector<const char *> &sCommands );

private:
	static std::vector<CBaseConCommand *> *s_pCommands;
	std::vector<CBaseConCommand *> *m_pCommands;
};

#endif // COMMANDMANAGER_H