#include <iostream>
#include "Global.h"
#include "GlobalManager.h"
#include "FileManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "RenderManager.h"
#include "AssetManager.h"

#include "Skybox.h"
#include "TestPlayer.h"
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

#include "BaseSprite.h"

CAnimatedProp *g_pAnimatedPropTest = NULL;

void CreateLevel( void );

bool CC_AnimatedPropTest( CTextItem *pCommand )
{
	g_pAnimatedPropTest->TransitionAnimation( 0, pCommand->GetTextTerm( 1 )->GetFloat() );
	return true;
}
CConCommand cc_animatedproptest( "animatedproptest", CC_AnimatedPropTest );

bool CC_ReloadTest( void )
{
	pEntityManager->ClearEntities();
	CreateLevel();
	return true;
}
CConCommand cc_reloadtest( "reloadtest", CC_ReloadTest );

void CreateLevel( void )
{
	{
		CTestPlayer *pNewPlayer;
		
		pNewPlayer = new CTestPlayer();
		pNewPlayer->SetPosition( glm::vec3( 0.0f, 0.0f, 1.0f ) );
		pEntityManager->AddEntity( pNewPlayer );
	}

	{
		CSkybox *pNewSkybox;

		pNewSkybox = new CSkybox();
		pNewSkybox->SetMaterial( "skybox.3mt" );
		pEntityManager->AddEntity( pNewSkybox );
	}

	{
		CStaticProp *pNewStaticProp;

		pNewStaticProp = new CStaticProp();
		pNewStaticProp->SetPosition( glm::vec3( 0.0f, 0.0f, -2.0f ) );
		pNewStaticProp->SetScale( g_vecOne * 500.0f );
		pNewStaticProp->SetModel( "plane_brick.3md" );
		pEntityManager->AddEntity( pNewStaticProp );

		pNewStaticProp = new CStaticProp();
		pNewStaticProp->SetPosition( glm::vec3( 0.0f, 20.0f, -2.0f ) );
		pNewStaticProp->SetScale( glm::vec3( 50.0f, 2.0f, 50.0f ) );
		pNewStaticProp->SetModel( "cube_brick.3md" );
		pEntityManager->AddEntity( pNewStaticProp );

		pNewStaticProp = new CStaticProp();
		pNewStaticProp->SetPosition( glm::vec3( 20.0f, 0.0f, -2.0f ) );
		pNewStaticProp->SetScale( glm::vec3( 2.0f, 50.0f, 50.0f ) );
		pNewStaticProp->SetModel( "cube_brick.3md" );
		pEntityManager->AddEntity( pNewStaticProp );

		pNewStaticProp = new CStaticProp();
		pNewStaticProp->SetPosition( glm::vec3( 0.0f, -20.0f, -2.0f ) );
		pNewStaticProp->SetScale( glm::vec3( 50.0f, 2.0f, 50.0f ) );
		pNewStaticProp->SetModel( "cube_brick.3md" );
		pEntityManager->AddEntity( pNewStaticProp );

		pNewStaticProp = new CStaticProp();
		pNewStaticProp->SetPosition( glm::vec3( -20.0f, 0.0f, -2.0f ) );
		pNewStaticProp->SetScale( glm::vec3( 2.0f, 50.0f, 50.0f ) );
		pNewStaticProp->SetModel( "cube_brick.3md" );
		pEntityManager->AddEntity( pNewStaticProp );
	}

	{
		CAnimatedProp *pNewAnimatedProp;

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( 4.0f, 0.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		g_pAnimatedPropTest = pNewAnimatedProp;

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( 4.0f, 5.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( 4.0f, 10.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( -4.0f, 0.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( -4.0f, 5.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( -4.0f, 10.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( 8.0f, 0.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( 8.0f, 5.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( 8.0f, 10.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( -8.0f, 0.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( -8.0f, 5.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( -8.0f, 10.0f, -2.0f ) );
		pNewAnimatedProp->SetScale( glm::vec3( g_vecOne * 0.1f ) );
		pNewAnimatedProp->SetModel( "boblampclean.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );

		pNewAnimatedProp = new CAnimatedProp();
		pNewAnimatedProp->SetPosition( glm::vec3( 0.0f, 3.0f, 4.0f ) );
		pNewAnimatedProp->SetModel( "testcharacter.3md" );
		pEntityManager->AddEntity( pNewAnimatedProp );
	}

	{
		CBaseSprite *pNewSprite;
		
		pNewSprite = new CBaseSprite();
		pNewSprite->SetPosition( glm::vec3( 0.0f, 0.0f, 1.0f ) );
		pNewSprite->SetMaterial( "brick.3mt" );
		pEntityManager->AddEntity( pNewSprite );
	}

	{
		CCSMShadowCamera *pNewCSMShadowCamera;

		pNewCSMShadowCamera = new CCSMShadowCamera();
		pNewCSMShadowCamera->SetInitialDistance( 6.0f );
		pNewCSMShadowCamera->SetDistanceFactor( 4.0f );
		pNewCSMShadowCamera->SetBlendDistance( 6.0f );
		pNewCSMShadowCamera->SetNearError( -160.0f );
		pNewCSMShadowCamera->SetFarError( 160.0f );
		pNewCSMShadowCamera->SetFadeNear( 180.0f );
		pNewCSMShadowCamera->SetFadeFar( 210.0f );
		pNewCSMShadowCamera->SetBlurRadius( 0.04f );
		pNewCSMShadowCamera->SetBaseSize( 2048 );
		pNewCSMShadowCamera->SetRenderPriority( 0 );
		pEntityManager->AddEntity( pNewCSMShadowCamera );

		CCSMLight *pNewCSMLight;
		
		pNewCSMLight = new CCSMLight();
		pNewCSMLight->SetRotation( glm::radians( glm::vec3( -60.0f, 0.0f, 30.0f ) ) );
		pNewCSMLight->SetShadowCamera( pNewCSMShadowCamera );
		pNewCSMLight->SetAmbient( g_vecOne * 0.1f );
		pNewCSMLight->SetDiffuse( g_vecOne * 0.9f );
		pNewCSMLight->SetSpecular( g_vecOne * 0.4f );
		pEntityManager->AddEntity( pNewCSMLight );
	}

	// CPointShadowCamera *pPointShadowCamera = new CPointShadowCamera( 0.1f, (-0.09f + std::sqrtf( 0.09f * 0.09f - 4.0f * 0.032f * (1.0f - 100.0f * std::fmaxf( std::fmaxf( 1.2f, 1.2f ), 1.2f )) )) / (2.0f * 0.032f), 60.0f, 80.0f, 0.04f, 512, 0, g_vecZero, g_vecZero, g_vecOne, true, true);
	// pEntityManager->AddEntity( pPointShadowCamera );
	// pEntityManager->AddEntity( new CPointLight( 1.0f, 0.09f, 0.032f, pPointShadowCamera, g_vecOne * 0.1f, g_vecOne * 1.2f, g_vecOne * 0.8f, glm::vec3( 0.0f, 0.0f, 1.0f ), g_vecZero, g_vecOne, true, true ) );

	// pEntityManager->AddEntity( new CSpotLightShadow( 50.0f, 60.0f, 0.02f, glm::radians( 70.0f ), glm::radians( 75.0f ), 1.0f, 0.09f, 0.032f, g_vecZero, g_vecOne * 0.9f, g_vecOne * 0.4f, glm::vec3( 0.0f, -12.0f, 0.0f ), g_vecZero, g_vecOne, true, true ) );
	// pEntityManager->AddEntity( new CSpotLightShadow( 50.0f, 60.0f, 0.01f, glm::radians( 44.0f ), glm::radians( 45.0f ), 1.0f, 0.09f, 0.032f, g_vecZero, g_vecOne * 4.0f, g_vecOne * 0.4f, glm::vec3( 0.0f, 0.0f, 0.0f ), glm::radians( glm::vec3( 0.0f, 0.0, 0.0f ) ), g_vecOne, true, true ) );

	// pEntityManager->AddEntity( new CDirLightShadow( 180.0f, 210.0f, 0.08f, 20.0f, -100.0f, 100.0f, g_vecOne * 0.1f, g_vecOne * 0.9f, g_vecOne * 0.4f, g_vecZero, glm::radians( glm::vec3( -70.0f, 0.0f, 30.0f ) ), g_vecOne, true, true ) );

	// pEntityManager->AddEntity( new CPointLightShadow( 0.01f, 1.0f, 0.09f, 0.032f, g_vecZero, g_vecOne * 4.0f, g_vecOne * 0.4f, glm::vec3( 0.0f, 0.0f, 0.0f ), glm::radians( glm::vec3( 0.0f, 0.0, 180.0f ) ), g_vecOne, true, true ) );
	
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

		while (!pGlobalValues->ShouldLoop());
		pGlobalValues->OnLoop();
	}

	DestroyGlobals();

	return EXIT_SUCCESS;
}