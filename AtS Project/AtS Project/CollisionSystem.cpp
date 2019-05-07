#include "stdafx.h"
#include "CollisionSystem.h"
#include "GameObjectsCollection.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "Utilities.h"


CollisionSystem::CollisionSystem() {}

CollisionSystem::~CollisionSystem() {}

void CollisionSystem::Init(const int* pCurrentSceneId)
{
	m_pCurrentSceneId = pCurrentSceneId;
	m_debugMode = false;
}

void CollisionSystem::Update()
{
	auto pDynamicColliders = GameObjectsCollection::get().collectionProvider().DynamicColliderCollection();
	auto pStaticColliders = GameObjectsCollection::get().collectionProvider().StaticColliderCollection();

	auto dynamicCollidersIt = pDynamicColliders->find(*m_pCurrentSceneId);
	auto staticCollidersIt = pStaticColliders->find(*m_pCurrentSceneId);

	if (dynamicCollidersIt == pDynamicColliders->end() && staticCollidersIt == pDynamicColliders->end()) { return; }

	Vector2 mtv;

	for (auto& layerIt : dynamicCollidersIt->second)
	{
		for (auto& dColliderIt : layerIt.second)
		{
			auto sCollidersLayerIt = staticCollidersIt->second.find(layerIt.first);

			if (sCollidersLayerIt == staticCollidersIt->second.end()) { break; }

			for (auto& sColliderIt : sCollidersLayerIt->second)
			{
				if (dColliderIt->Intersect(sColliderIt, mtv))
				{
					OutputDebugStringA("Collide : ");
					OutputDebugStringA(dColliderIt->owner()->tag().c_str());
					OutputDebugStringA(" ");
					OutputDebugStringA(sColliderIt->owner()->tag().c_str());
					OutputDebugStringA("MTV : ");
					OutputDebugStringA(std::to_string(mtv.x).c_str());
					OutputDebugStringA(" ");
					OutputDebugStringA(std::to_string(mtv.y).c_str());
					OutputDebugStringA("\n");

					//dColliderIt->owner()->transform.Translate(mtv);
				}
			}
		}
	}

	for (auto& layerIt : dynamicCollidersIt->second)
	{
		for (auto dColliderIt1 = layerIt.second.begin(); dColliderIt1 != layerIt.second.end() - 1; ++dColliderIt1)
		{
			for (auto dColliderIt2 = layerIt.second.begin() + 1; dColliderIt2 != layerIt.second.end(); ++dColliderIt2)
			{
				if ((*dColliderIt1)->Intersect((*dColliderIt2), mtv))
				{
					OutputDebugStringA("\n");
					OutputDebugStringA("Collide : ");
					OutputDebugStringA((*dColliderIt1)->owner()->tag().c_str());
					OutputDebugStringA(" ");
					OutputDebugStringA((*dColliderIt2)->owner()->tag().c_str());
					OutputDebugStringA("MTV : ");
					OutputDebugStringA(std::to_string(mtv.x).c_str());
					OutputDebugStringA(" ");
					OutputDebugStringA(std::to_string(mtv.y).c_str());
					OutputDebugStringA("\n");
				}
			}
		}
	}
}

void CollisionSystem::SetDefaultColliderDynamicMode(COLLIDER_DYNAMIC dynamic)
{
	GameObjectsCollection::get().systemRequests().SetDefaultColliderDynamic(dynamic);
	m_defaultColliderDynamicMode = dynamic;
}


void CollisionSystem::SetDebugMode(bool mode) { m_debugMode = mode; }

bool CollisionSystem::OnDebugMode() { return m_debugMode; }