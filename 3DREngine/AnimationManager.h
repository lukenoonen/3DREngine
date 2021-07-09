#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "Global.h"
#include "Animation.h"
#include "AnimationController.h"

class CAnimationManager
{
public:
	DECLARE_CLASS_NOBASE( CAnimationManager );

	CAnimationManager();
	~CAnimationManager();

	void Reset( void );

	CAnimationController *GetAnimationController( const char *sPath );

	CAnimation *GetAnimation( const char *sPath );

private:
	CAnimation *CreateAnimation( const char *sPath );

	std::vector<CAnimation *> m_pAnimations;
	std::vector<CAnimationController *> m_pAnimationControllers;
};

#endif // ANIMATIONMANAGER_H