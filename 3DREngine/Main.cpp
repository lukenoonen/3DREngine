#include "Global.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "RenderManager.h"

int main()
{
	CreateGlobals();

	pCommandManager->ProcessCommand( "exec autoexec;" );

	while (!pTimeManager->MarkedForQuit())
	{
		pInputManager->OnLoop();
		pEntityManager->OnLoop();
		pRenderManager->OnLoop();

		while (!pTimeManager->ShouldLoop());
		pTimeManager->OnLoop();
	}

	DestroyGlobals();

	return EXIT_SUCCESS;
}