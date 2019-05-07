#pragma once
#include "GameObjectsCollection.h"
#include "GameObject.h"
#include "ViewSystem.h"
#include "Actor.h"

class AnimationTestScene
{
public:
	AnimationTestScene() {}
	~AnimationTestScene() {}

	void Init()
	{
		ViewSystem* view = &ViewSystem::get();
		m_pGameObjectCollection = &GameObjectsCollection::get();

		view->ZoomView(-0.4f);

		GameObject* animationTester = m_pGameObjectCollection->gameObject<Actor>("Actor");
	}

private:
	GameObjectsCollection* m_pGameObjectCollection;
};


