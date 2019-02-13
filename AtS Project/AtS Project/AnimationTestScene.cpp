#include "stdafx.h"
#include "AnimationTestScene.h"
#include "ViewSystem.h"
#include "Actor.h"

AnimationTestScene::AnimationTestScene()
{
}

AnimationTestScene::~AnimationTestScene() {}

void AnimationTestScene::Init()
{
	ViewSystem* view = &ViewSystem::get();
	m_pGameObjectCollection = &GameObjectsCollection::get();

	view->ZoomView(-0.4f);

	GameObject* animationTester = m_pGameObjectCollection->gameObject<Actor>("Actor");

}