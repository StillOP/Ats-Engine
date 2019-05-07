#include "stdafx.h"
#include "GameObjectsCollection.h"
#include "GameObject.h"


GameObjectsCollection::GameObjectsCollection() {}

GameObjectsCollection::~GameObjectsCollection() {}

void GameObjectsCollection::Init(const int* pCurrentSceneId)
{
	m_nextObjectId = 0;

	m_pCurrentSceneId = pCurrentSceneId;

	m_pcomponentsMap.emplace(std::type_index(typeid(SpriteRenderer)), &m_spriteRenderers);
	m_pcomponentsMap.emplace(std::type_index(typeid(AnimationController)), &m_animationControllers);
	m_pcomponentsMap.emplace(std::type_index(typeid(StateController)), &m_stateControllers);
	m_pcomponentsMap.emplace(std::type_index(typeid(BoxCollider)), &m_boxColliders);
	m_pcomponentsMap.emplace(std::type_index(typeid(RigidBody)), &m_rigidBodys);
}

void GameObjectsCollection::Update()
{
	if (!m_bindedUpdates.empty())
	{
		for (auto updateIt = m_bindedUpdates.begin(); updateIt != m_bindedUpdates.end();)
		{
			(*updateIt->second)->Update();

			if ((*updateIt->second)->haviour()->m_unbindUpdate)
			{
				updateIt->second = nullptr;
				updateIt = m_bindedUpdates.erase(updateIt);
			}
			else { ++updateIt; }
		}
	}
}

void GameObjectsCollection::StateUpdate()
{
	if (!m_bindedStatesUpdates.empty() && m_bindedStatesUpdates.count(*m_pCurrentSceneId) == 1)
	{
		for (auto updateIt = m_bindedStatesUpdates[*m_pCurrentSceneId].begin(); updateIt != m_bindedStatesUpdates[*m_pCurrentSceneId].end();)
		{
			auto stateController = m_stateControllers[updateIt->first];

			(*updateIt->second)->OnStateUpdate(stateController.currentState());

			if ((*updateIt->second)->haviour()->m_unbindUpdate)
			{
				updateIt->second = nullptr;
				updateIt = m_bindedStatesUpdates[*m_pCurrentSceneId].erase(updateIt);
			}
			else { ++updateIt; }
		}
	}
}

GameObject* GameObjectsCollection::gameObject(std::string name)
{
	if (*m_pCurrentSceneId == -1) { return nullptr; }

	auto itr = m_gameObjectsTag.find(name);
	if (itr != m_gameObjectsTag.end()) 
	{
		for (auto& sceneIt : m_gameObjectsCollection)
		{
			auto objectIt = sceneIt.second.find(itr->second);
			if (objectIt != sceneIt.second.end()) { return &objectIt->second; }
		}
	}

	GameObject object = GameObject(name);

	object.m_id = m_nextObjectId;
	object.m_sceneId = *m_pCurrentSceneId;

	m_gameObjectsTag.emplace(name, m_nextObjectId);
	m_nextObjectId++;
	
	auto pObject = &m_gameObjectsCollection[*m_pCurrentSceneId].emplace(object.m_id, std::move(object)).first->second;
	pObject->transform.owner(pObject);

	return pObject;
}


GameObject* GameObjectsCollection::Find(std::string name)
{
	auto tagIt = m_gameObjectsTag.find(name);

	if (tagIt == m_gameObjectsTag.end()) { return nullptr; }

	auto objectIt = m_gameObjectsCollection[*m_pCurrentSceneId].find(tagIt->second);

	if (objectIt != m_gameObjectsCollection[*m_pCurrentSceneId].end()) { return &objectIt->second; }

	for (auto& sceneIt : m_gameObjectsCollection)
	{
		if (sceneIt.first == *m_pCurrentSceneId) { continue; }

		auto objectIt = sceneIt.second.find(tagIt->second);

		if (objectIt != sceneIt.second.end()) { return &objectIt->second; }
	}

	return nullptr;
}

const std::map<unsigned int, std::unordered_map<unsigned int, GameObject>>* GameObjectsCollection::gameObjectCollections()
{
	return &m_gameObjectsCollection;
}

void GameObjectsCollection::Destroy(std::string name)
{
	auto tagIt = m_gameObjectsTag.find(name);
	if (tagIt == m_gameObjectsTag.end()) { return; }

	auto object = Find(name);
	unsigned int id = tagIt->second;

	RemoveComponent<AnimationController>(id);
	RemoveComponent<StateController>(id);

	if (object->HasComponent<SpriteRenderer>())
	{
		//Delete pointer in SpriteRenderer component collection

		auto pSpriteRendererCollection = &m_forRenderSystem[*m_pCurrentSceneId][object->layer()];
		bool find = false;

		for (auto pSpriteRendererIt = pSpriteRendererCollection->begin(); pSpriteRendererIt != pSpriteRendererCollection->end(); ++pSpriteRendererCollection)
		{
			if (*pSpriteRendererIt == object->GetComponent<SpriteRenderer>())
			{
				*pSpriteRendererIt = nullptr;
				pSpriteRendererCollection->erase(pSpriteRendererIt);
				find = true;
			}
		}

		if (!find)
		{
			for (auto& sceneIt : m_forRenderSystem)
			{
				auto vectorIt = sceneIt.second.find(object->layer());

				if (vectorIt != sceneIt.second.end())
				{
					for (auto pSpriteRendererIt = vectorIt->second.begin(); pSpriteRendererIt != vectorIt->second.end(); ++pSpriteRendererIt)
					{
						if (*pSpriteRendererIt == object->GetComponent<SpriteRenderer>())
						{
							*pSpriteRendererIt = nullptr;
							vectorIt->second.erase(pSpriteRendererIt);
						}
					}
				}
			}
		}

		RemoveComponent<SpriteRenderer>(id);
	}

	//Delete behaviour component and binded update function

	auto behaviourIt = m_behaviours.find(id);
	if (behaviourIt != m_behaviours.end()) 
	{ 
		behaviourIt->second->owner(nullptr);

		auto pbehaviourIt = m_bindedUpdates.find(id);

		if (pbehaviourIt != m_bindedUpdates.end())
		{
			pbehaviourIt->second = nullptr;
			m_bindedUpdates.erase(id);
		}

		delete behaviourIt->second;
		m_behaviours.erase(id);
	}

	
	auto objectIt = m_gameObjectsCollection[*m_pCurrentSceneId].find(id);

	if (objectIt != m_gameObjectsCollection[*m_pCurrentSceneId].end()) 
	{ 
		m_gameObjectsCollection[*m_pCurrentSceneId].erase(id); 
	}
	else
	{
		for (auto& sceneIt : m_gameObjectsCollection)
		{
			if (sceneIt.first == *m_pCurrentSceneId) { continue; }

			auto objectIt = sceneIt.second.find(id);

			if (objectIt != sceneIt.second.end()) { sceneIt.second.erase(id); }
		}
	}
}

void GameObjectsCollection::SetObjectLayer(std::string& tag, unsigned int layer)
{
	GameObject* object = Find(tag);

	if (object == nullptr) { return; }


	if (object->HasComponent<SpriteRenderer>())
	{
		auto pSpriteRenderer = object->GetComponent<SpriteRenderer>();
		auto pVector = &m_forRenderSystem.at(object->scene()).at(object->layer());

		for (auto itr = pVector->begin(); itr != pVector->end(); ++itr)
		{
			if (*itr == pSpriteRenderer)
			{
				*itr = nullptr;
				pVector->erase(itr);
			}
		}

		m_forRenderSystem.at(object->scene()).at(layer).push_back(pSpriteRenderer);
	}

	object->layer(layer);
}

void GameObjectsCollection::AddInRenderSystemCollection(GameObject* object)
{
	auto pSpriteRenderer = object->GetComponent<SpriteRenderer>();
	
	m_forRenderSystem[object->scene()][object->layer()].push_back(pSpriteRenderer);

}

void GameObjectsCollection::AddInCollisionSystemCollection(GameObject* object)
{
	Collider* pCollider = nullptr;
	
	auto colliders = m_request.GetDefaultColliderDynamic() == COLLIDER_DYNAMIC::DYNAMIC ? 
		&(m_dynamicColliders[object->scene()][object->layer()]) : &(m_staticColliders[object->scene()][object->layer()]);
	
	if (object->HasComponent<BoxCollider>())
	{
		pCollider = object->GetComponent<BoxCollider>();
	}

	pCollider->colliderDynamic(m_request.GetDefaultColliderDynamic());
	colliders->push_back(pCollider);

	m_collidersForRendering[*m_pCurrentSceneId].push_back(pCollider);
}

void GameObjectsCollection::BindStateUpdate(GameObject* object)
{
	unsigned int id = m_gameObjectsTag.find(object->tag())->second;
	auto controller = object->GetComponent<StateController>();

	if (m_behaviours.count(id) == 1)
	{
		controller->SetBehaviour(&m_behaviours[id]);
		m_bindedStatesUpdates[*m_pCurrentSceneId].emplace(id, &m_behaviours[id]);
	}
}

void GameObjectsCollection::InitRigidBody(GameObject* object)
{
	auto rigidBody = object->GetComponent<RigidBody>();
	rigidBody->transform(&(object->transform));

	m_rigidBodysCollection[*m_pCurrentSceneId].push_back(rigidBody);
}

void GameObjectsCollection::ClearScene(unsigned int sceneId)
{
	for (auto& objectIt : m_gameObjectsCollection[sceneId])
	{
		Destroy(objectIt.second.tag());
	}

	m_gameObjectsCollection[sceneId].clear();
}


void GameObjectsCollection::SystemRequests::SetObjectCollisionDynamic(GameObject* object, COLLIDER_DYNAMIC dynamic)
{
	Collider * pCollider = nullptr;

	if (object->HasComponent<BoxCollider>()) { pCollider = object->GetComponent<BoxCollider>(); }

	if (pCollider->colliderDynamic() == dynamic) { return; }

	auto pDynamicColliders = &(GameObjectsCollection::get().m_dynamicColliders[object->scene()][object->layer()]);
	auto pStaticColliders = &(GameObjectsCollection::get().m_staticColliders[object->scene()][object->layer()]);

	auto toRemoveOn = dynamic == COLLIDER_DYNAMIC::DYNAMIC ? pStaticColliders : pDynamicColliders;
	auto toAddOn = dynamic == COLLIDER_DYNAMIC::DYNAMIC ? pDynamicColliders : pStaticColliders;

	for (auto colliderIt = toRemoveOn->begin(); colliderIt != toRemoveOn->end(); ++colliderIt)
	{
		if ((*colliderIt)->owner()->id() == object->id())
		{
			colliderIt = toRemoveOn->erase(colliderIt);
			toAddOn->push_back(pCollider);

			return;
		}
	}
}