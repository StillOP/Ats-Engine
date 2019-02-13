#include "stdafx.h"
#include "GameWorld.h"
#include "SceneSystem.h"
#include "AnimationTestScene.h"

void GameWorld::Init()
{
	//Start implement the game here.
	//Add at least one scene and switch to its

	SceneSystem* scnSys = &SceneSystem::get();
	
	scnSys->AddScene("Animation Test Scene", &AnimationTestScene::Init, AnimationTestScene());
	scnSys->SwitchTo("Animation Test Scene");

}