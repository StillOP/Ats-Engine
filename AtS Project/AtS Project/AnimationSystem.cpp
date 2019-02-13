#include "stdafx.h"
#include "AnimationSystem.h"
#include "GameObject.h"
#include "Time.h"


AnimationSystem::AnimationSystem() {}

AnimationSystem::~AnimationSystem() {}

void AnimationSystem::Init() {}

void AnimationSystem::Update(double elapsed)
{
	auto pGameObjectsCollection = GameObjectsCollection::get().gameObjectCollections();
	unsigned int currentScene = SceneSystem::get().currentScene()->id;
	
	if (pGameObjectsCollection->empty()) { return; }

	auto pCurrentCollection = &pGameObjectsCollection->at(currentScene);

	if (pCurrentCollection->empty()) { return; }

	for (auto& itr : *pCurrentCollection)
	{
		if (itr.second.HasComponent<AnimationController>())
		{
			auto controller = itr.second.GetComponent<AnimationController>();
			auto animation = controller->currentAniamtion();

			if (!animation) { continue; }

			if (animation->elapsed >= animation->frameTime)
			{
				controller->NextFrame();
				animation->elapsed = 0.0;
			}
			else
			{
				animation->elapsed += elapsed;
			}
		}
	}
}