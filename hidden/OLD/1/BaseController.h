#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "Global.h"

template <class T>
class CBaseControllerSequence
{
public:
	DECLARE_CLASS_NOBASE( CBaseControllerSequence );

	CBaseControllerSequence( const std::vector<T *> &pAssets, const std::vector<float> &flAssetTimes );

	void Reset( void );
	float Update( float flTime );

	unsigned int GetAssetCount( void );
	T *GetAsset( void );

private:
	std::vector<T *> m_pAssets;
	std::vector<float> m_flAssetTimes;
	
	unsigned int m_uiCurrentAsset;
	float m_flCurrentAssetTime;
};

template <class T>
class CBaseController
{
public:
	DECLARE_CLASS_NOBASE( CBaseController );

	CBaseController( const std::vector<CBaseControllerSequence<T> *> &pControllerSequences );
	~CBaseController();

	void Continue( unsigned int uiNumberOfContinues );
	void Skip( void );

	void Play( unsigned int uiIndex );
	void Queue( unsigned int uiIndex );

	void Update( void );

	T *GetAsset( void );

private:
	std::vector<CBaseControllerSequence<T> *> m_pControllerSequences;
	std::queue<unsigned int> m_uiSequenceQueue;
	unsigned int m_uiNumberOfContinues;
};

#endif // BASECONTROLLER_H