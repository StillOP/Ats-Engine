#pragma once
#include "GameObjectsCollection.h"
#include "GameObject.h"
#include "ResourceSystem.h"
#include "InputSystem.h"

class AnimationTestScene
{
public:
	AnimationTestScene();
	~AnimationTestScene();

	void Init();

private:
	GameObjectsCollection* m_pGameObjectCollection;
};


