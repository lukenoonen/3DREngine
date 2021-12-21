#include <iostream>
#include "Global.h"
#include "GlobalManager.h"
#include "FileManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "RenderManager.h"

#include "Skybox.h"
#include "Player.h"
#include "PerspectivePlayerCamera.h"
#include "StaticProp.h"
#include "AnimatedProp.h"
#include "CSMLight.h"
#include "CSMShadowCamera.h"
#include "DirLight.h"
#include "DirShadowCamera.h"
#include "PointLight.h"
#include "PointShadowCamera.h"
#include "SpotLight.h"
#include "SpotShadowCamera.h"

CAnimatedProp *g_pAnimatedPropTest = NULL;

bool CC_AnimatedPropTest( CTextItem *pCommand )
{
	g_pAnimatedPropTest->TransitionAnimation( 0, pCommand->GetTextTerm( 1 )->GetFloat() );
	return true;
}
CConCommand cc_animatedproptest( "animatedproptest", CC_AnimatedPropTest );

void CreateLevel( void )
{
	CPerspectivePlayerCamera *pPlayerCamera = new CPerspectivePlayerCamera( 1, g_vecZero, g_vecZero, g_vecOne, true, true );
	CPlayer *pPlayer = new CPlayer( pPlayerCamera, glm::vec3( 0.0f, 0.0f, -100.0f ), g_vecZero, g_vecOne, true, true );
	pPlayerCamera->SetPosition( pPlayer->GetPosition() );
	pPlayerCamera->SetRotation( pPlayer->GetRotation() );
	pPlayerCamera->SetScale( pPlayer->GetScale() );
	pPlayerCamera->SetParent( pPlayer );
	pEntityManager->AddEntity( pPlayerCamera );
	pEntityManager->AddEntity( pPlayer );
	pEntityManager->AddEntity( new CSkybox( "skybox.3mt", true, true ) );
	pEntityManager->AddEntity( new CStaticProp( "plane_brick.3md", glm::vec3( 0.0f, 0.0f, -2.0f ), g_vecZero, g_vecOne * 500.0f, true, true ) );
	pEntityManager->AddEntity( new CStaticProp( "cube_brick.3md", glm::vec3( 0.0f, 20.0f, -2.0f ), g_vecZero, glm::vec3( 50.0f, 2.0f, 50.0f ), true, true ) );
	pEntityManager->AddEntity( new CStaticProp( "cube_brick.3md", glm::vec3( 20.0f, 0.0f, -2.0f ), g_vecZero, glm::vec3( 2.0f, 50.0f, 50.0f ), true, true ) );
	pEntityManager->AddEntity( new CStaticProp( "cube_brick.3md", glm::vec3( 0.0f, -20.0f, -2.0f ), g_vecZero, glm::vec3( 50.0f, 2.0f, 50.0f ), true, true ) );
	pEntityManager->AddEntity( new CStaticProp( "cube_brick.3md", glm::vec3( -20.0f, 0.0f, -2.0f ), g_vecZero, glm::vec3( 2.0f, 50.0f, 50.0f ), true, true ) );

	g_pAnimatedPropTest = new CAnimatedProp( "boblampclean.3md", glm::vec3( 4.0f, 0.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true );
	
	pEntityManager->AddEntity( g_pAnimatedPropTest );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( 4.0f, 5.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( 4.0f, 10.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( -4.0f, 0.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( -4.0f, 5.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( -4.0f, 10.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( 8.0f, 0.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( 8.0f, 5.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( 8.0f, 10.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( -8.0f, 0.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( -8.0f, 5.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );
	pEntityManager->AddEntity( new CAnimatedProp( "boblampclean.3md", glm::vec3( -8.0f, 10.0f, -2.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne * 0.1f, true, true ) );

	pEntityManager->AddEntity( new CAnimatedProp( "testcharacter.3md", glm::vec3( 0.0f, 3.0f, 4.0f ), glm::radians( glm::vec3( 0.0f, 0.0f, 0.0f ) ), g_vecOne, true, true ) );

	CCSMShadowCamera *pCSMShadowCamera = new CCSMShadowCamera(3.0f, 8.0f, 4.0f, -60.0f, 60.0f, 180.0f, 210.0f, 0.04f, 2048, 0, g_vecZero, glm::radians(glm::vec3(-70.0f, 0.0f, 30.0f)), g_vecOne, true, true);
	pEntityManager->AddEntity( pCSMShadowCamera );
	pEntityManager->AddEntity( new CCSMLight( pCSMShadowCamera, g_vecOne * 0.1f, g_vecOne * 0.9f, g_vecOne * 0.4f, g_vecZero, glm::radians( glm::vec3( -70.0f, 0.0f, 30.0f ) ), g_vecOne, true, true ) );
	

	CPointShadowCamera *pPointShadowCamera = new CPointShadowCamera( 0.1f, (-0.09f + std::sqrtf( 0.09f * 0.09f - 4.0f * 0.032f * (1.0f - 100.0f * std::fmaxf( std::fmaxf( 1.2f, 1.2f ), 1.2f )) )) / (2.0f * 0.032f), 60.0f, 80.0f, 0.04f, 512, 0, g_vecZero, g_vecZero, g_vecOne, true, true);
	pEntityManager->AddEntity( pPointShadowCamera );
	pEntityManager->AddEntity( new CPointLight( 1.0f, 0.09f, 0.032f, pPointShadowCamera, g_vecOne * 0.1f, g_vecOne * 1.2f, g_vecOne * 0.8f, glm::vec3( 0.0f, 0.0f, 1.0f ), g_vecZero, g_vecOne, true, true ) );

//	CPointLightShadow *pTest = new CPointLightShadow( 50.0f, 60.0f, 0.01f, 1.0f, 0.09f, 0.032f, g_vecZero, g_vecOne * 4.0f, g_vecOne * 0.4f, glm::vec3( 0.0f, 5.0f, 0.0f ), glm::radians( glm::vec3( 0.0f, 0.0, 180.0f ) ), g_vecOne, true, true );
	//CSpotLightShadow *pTest = new CSpotLightShadow( 50.0f, 60.0f, 0.02f, glm::radians( 40.0f ), glm::radians( 45.0f ), 1.0f, 0.09f, 0.032f, g_vecZero, g_vecOne * 4.0f, g_vecOne * 0.4f, glm::vec3( 0.0f, 2.0f, -1.0f ), glm::radians( glm::vec3( 10.0f, 0.0, 0.0f ) ), g_vecOne, true, true );
	//pEntityManager->AddEntity( pTest );
	// pTest->SetParent( pPlayer );

	//pEntityManager->AddEntity( new CSpotLightShadow( 50.0f, 60.0f, 0.02f, glm::radians( 70.0f ), glm::radians( 75.0f ), 1.0f, 0.09f, 0.032f, g_vecZero, g_vecOne * 0.9f, g_vecOne * 0.4f, glm::vec3( 0.0f, -12.0f, 0.0f ), g_vecZero, g_vecOne, true, true ) );
	//pEntityManager->AddEntity( new CSpotLightShadow( 50.0f, 60.0f, 0.01f, glm::radians( 44.0f ), glm::radians( 45.0f ), 1.0f, 0.09f, 0.032f, g_vecZero, g_vecOne * 4.0f, g_vecOne * 0.4f, glm::vec3( 0.0f, 0.0f, 0.0f ), glm::radians( glm::vec3( 0.0f, 0.0, 0.0f ) ), g_vecOne, true, true ) );

	// pEntityManager->AddEntity( new CDirLightShadow( 180.0f, 210.0f, 0.08f, 20.0f, -100.0f, 100.0f, g_vecOne * 0.1f, g_vecOne * 0.9f, g_vecOne * 0.4f, g_vecZero, glm::radians( glm::vec3( -70.0f, 0.0f, 30.0f ) ), g_vecOne, true, true ) );

	//pEntityManager->AddEntity( new CPointLightShadow( 0.01f, 1.0f, 0.09f, 0.032f, g_vecZero, g_vecOne * 4.0f, g_vecOne * 0.4f, glm::vec3( 0.0f, 0.0f, 0.0f ), glm::radians( glm::vec3( 0.0f, 0.0, 180.0f ) ), g_vecOne, true, true ) );
	
	//pEntityManager->AddEntity( new CBaseModeled( "brick_master.3mc", "cube.3md", glm::vec3( 0.0f, 0.0f, 2.0f ), g_vecZero, glm::vec3( 1.0f, 1.0f, 1.0f ), true, true ) );

	//pEntityManager->AddEntity( new CMaterialHUDElement( "HUDColorTest.3mt", g_vecZero, g_vecOne * 0.2f, 1.0f, true, true ) );
}

int main()
{
	CreateGlobals();

	pCommandManager->ProcessCommand( "exec autoexec.cfg;" );

	CreateLevel();

	while (!pGlobalValues->MarkedForQuit())
	{
		pInputManager->OnLoop();
		pEntityManager->OnLoop();
		pRenderManager->OnLoop();

		while (!pGlobalValues->ShouldLoop()); // TODO: see if this changes anything at all
		pGlobalValues->OnLoop(); // TODO: see if this changes anything at all
	}

	DestroyGlobals();
	return EXIT_SUCCESS;
}