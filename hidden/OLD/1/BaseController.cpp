#include "BaseController.h"
#include "GlobalManager.h"
#include "Animation.h"
#include "Material.h"
#include "Model.h"
#include "Texture.h"

template class CBaseControllerSequence<CAnimation>;
template class CBaseControllerSequence<CMaterial>;
template class CBaseControllerSequence<CModel>;
template class CBaseControllerSequence<CTexture>;

template <class T>
CBaseControllerSequence<T>::CBaseControllerSequence<T>( const std::vector<T *> &pAssets, const std::vector<float> &flAssetTimes )
{
	m_pAssets = pAssets;
	m_flAssetTimes = flAssetTimes;

	Reset();
}

template <class T>
void CBaseControllerSequence<T>::Reset( void )
{
	m_uiCurrentAsset = 0;
	m_flCurrentAssetTime = 0.0f;
}

template <class T>
float CBaseControllerSequence<T>::Update( float flTime )
{
	m_flCurrentAssetTime += flTime;
	while (m_flCurrentAssetTime >= m_flAssetTimes[m_uiCurrentAsset])
	{
		m_flCurrentAssetTime -= m_flAssetTimes[m_uiCurrentAsset];
		m_uiCurrentAsset++;

		if (m_uiCurrentAsset >= (unsigned int)m_pAssets.size())
			return m_flCurrentAssetTime;
	}

	return 0.0f;
}

template <class T>
unsigned int CBaseControllerSequence<T>::GetAssetCount( void )
{
	return (unsigned int)m_pAssets.size();
}

template <class T>
T *CBaseControllerSequence<T>::GetAsset( void )
{
	return m_pAssets[m_uiCurrentAsset];
}

template class CBaseController<CAnimation>;
template class CBaseController<CMaterial>;
template class CBaseController<CModel>;
template class CBaseController<CTexture>;

template <class T>
CBaseController<T>::CBaseController<T>( const std::vector<CBaseControllerSequence<T> *> &pControllerSequences )
{
	m_pControllerSequences = pControllerSequences;
	m_uiNumberOfContinues = 0;
}

template <class T>
CBaseController<T>::~CBaseController<T>()
{
	for (unsigned int i = 0; i < (unsigned int)m_pControllerSequences.size(); i++)
		delete m_pControllerSequences[i];
}

template <class T>
void CBaseController<T>::Continue( unsigned int uiNumberOfContinues )
{
	m_uiNumberOfContinues = uiNumberOfContinues;
}

template <class T>
void CBaseController<T>::Skip( void )
{
	if (m_uiNumberOfContinues > 0)
		m_uiNumberOfContinues--;

	m_uiSequenceQueue.pop();
}

template <class T>
void CBaseController<T>::Play( unsigned int uiIndex )
{
	if (m_uiNumberOfContinues > 0)
		m_uiNumberOfContinues = 0;

	std::queue<unsigned int>().swap( m_uiSequenceQueue );
	m_uiSequenceQueue.push( uiIndex );
}

template <class T>
void CBaseController<T>::Queue( unsigned int uiIndex )
{
	m_uiSequenceQueue.push( uiIndex );
}

template <class T>
void CBaseController<T>::Update( void )
{
	if (m_uiSequenceQueue.empty())
		return;

	float flTime = pGlobalValues->GetFrameTime();
	if (flTime > 0.0f)
	{
		while (true)
		{
			CBaseControllerSequence<T> *pControllerSequence = m_pControllerSequences[m_uiSequenceQueue.front()];
			flTime = pControllerSequence->Update( flTime );
			if (flTime <= 0.0f)
				break;

			pControllerSequence->Reset();

			bool bShouldContinue = false;
			if (m_uiNumberOfContinues > 0)
			{
				m_uiNumberOfContinues--;
				bShouldContinue = true;
			}

			if (bShouldContinue)
			{
				m_uiSequenceQueue.pop();
				if (m_uiSequenceQueue.empty())
					break;
			}
		}
	}
}

template <class T>
T *CBaseController<T>::GetAsset( void )
{
	if (m_uiSequenceQueue.empty())
		return NULL;

	return m_pControllerSequences[m_uiSequenceQueue.front()]->GetAsset();
}