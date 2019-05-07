#pragma once
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include "AnimationController.h"
#include "SpriteRenderer.h"
#include "StateController.h"
#include "RigidBody.h"
#include "Behaviour.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SceneSystem.h"

#include "COLLIDER_DYNAMIC.h"

class GameObjectsCollection
{
	friend class GameObject;
public:
	~GameObjectsCollection();

	void Init(const int* pCurrentSceneId);
	void Update();
	void StateUpdate();

	static GameObjectsCollection& get()
	{
		static GameObjectsCollection sCollection;
		return sCollection;
	}

	GameObject* gameObject(std::string name = "");

	template<class T>
	GameObject* gameObject(std::string name = "")
	{
		GameObject* object = gameObject(name);

		auto behaviour = &m_behaviours.emplace(m_nextObjectId - 1, new T(Haviour())).first->second;
		(*behaviour)->owner(object);

		m_bindedUpdates.emplace(m_nextObjectId -1, behaviour);

		(*behaviour)->Init();

		return object;
	}

	GameObject* Find(std::string name);
	void Destroy(std::string name);

	void ClearScene(unsigned int sceneId);
	
	const std::map<unsigned int, std::unordered_map<unsigned int, GameObject>>* gameObjectCollections();

private:
	GameObjectsCollection();

	template<class T>
	std::map<unsigned int, T>* GetComponentMaps()
	{
		for (auto& itr : m_pcomponentsMap)
		{
			if (itr.first == std::type_index(typeid(T)))
			{
				return (std::map<unsigned int, T>*)itr.second;
			}
		}
		return nullptr;
	}
	
	template<class T>
	T* AddComponent(GameObject* owner, unsigned int id)
	{
		T component;
		component.owner(owner);

		std::map<unsigned int, T>* pmap = GetComponentMaps<T>();

		if (!pmap) { return nullptr; }
		if (pmap->find(id) != pmap->end()) { return nullptr; }

		auto pComponent =  &pmap->emplace(id, component).first->second;

		if (std::is_same<T, SpriteRenderer>::value) { AddInRenderSystemCollection(owner); } 
		if (std::is_same<T, BoxCollider>::value) { AddInCollisionSystemCollection(owner); }
		if (std::is_same<T, StateController>::value) { BindStateUpdate(owner); }
		if (std::is_same<T, RigidBody>::value) { InitRigidBody(owner); }

		return pComponent;
	}

	template<class T>
	T* GetComponent(unsigned int id)
	{
		std::map<unsigned int, T>* pmap = GetComponentMaps<T>();

		if (!pmap) { return NULL; }

		auto itr = pmap->find(id);

		return itr != pmap->end() ? &itr->second : NULL;
	}

	template<class T>
	bool HasComponent(unsigned int id)
	{
		std::map<unsigned int, T>* pmap = GetComponentMaps<T>();

		if (!pmap) { return false; }

		return pmap->count(id) > 0 ? true : false;
	}
	
	template<class T>
	void RemoveComponent(unsigned int id)
	{
		std::map<unsigned int, T>* pmap = GetComponentMaps<T>();

		if (!pmap) { return; }

		auto itr = pmap->find(id);
		if (itr == pmap->end()) { return; }

		itr->second.owner(nullptr);
		pmap->erase(id);
	}

	void SetObjectLayer(std::string& tag, unsigned int layer);
	void AddInRenderSystemCollection(GameObject* object);
	void AddInCollisionSystemCollection(GameObject* object);
	void BindStateUpdate(GameObject* object);
	void InitRigidBody(GameObject* object);
	
	std::map<unsigned int, std::unordered_map<unsigned int, GameObject>> m_gameObjectsCollection;
	std::map<std::string, unsigned int> m_gameObjectsTag;

	std::map<unsigned int, AnimationController> m_animationControllers;
	std::map<unsigned int, SpriteRenderer> m_spriteRenderers;
	std::map<unsigned int, StateController> m_stateControllers;
	std::map<unsigned int, BoxCollider> m_boxColliders;
	std::map<unsigned int, RigidBody> m_rigidBodys;
	std::map<unsigned int, Behaviour*> m_behaviours;

	std::map<std::type_index, void*> m_pcomponentsMap;

	std::unordered_map<unsigned int, Behaviour**> m_bindedUpdates;
	std::unordered_map<unsigned int, std::unordered_map<unsigned int, Behaviour**>> m_bindedStatesUpdates;

	std::map<unsigned int, std::map<unsigned int, std::vector<SpriteRenderer*>>> m_forRenderSystem;
	std::unordered_map<unsigned int, std::unordered_map<unsigned int, std::vector<Collider*>>> m_staticColliders;
	std::unordered_map<unsigned int, std::unordered_map<unsigned int, std::vector<Collider*>>> m_dynamicColliders;
	std::unordered_map<unsigned int, std::vector<Collider*>> m_collidersForRendering;
	std::unordered_map<unsigned int, std::vector<RigidBody*>> m_rigidBodysCollection;

	unsigned int m_nextObjectId;
	const int* m_pCurrentSceneId;

	class CollectionProvider
	{
		friend class RenderSystem;
		friend class CollisionSystem;
		friend class RigidBodySystem;
	public:

	private:

		static const std::map<unsigned int, std::map<unsigned int, std::vector<SpriteRenderer*>>>* SpriteRendererCollection()
		{
			return &(GameObjectsCollection::get().m_forRenderSystem);
		}

		static std::unordered_map<unsigned int, std::unordered_map<unsigned int, std::vector<Collider*>>>* StaticColliderCollection()
		{
			return &(GameObjectsCollection::get().m_staticColliders);
		}

		static std::unordered_map<unsigned int, std::unordered_map<unsigned int, std::vector<Collider*>>>* DynamicColliderCollection()
		{
			return &(GameObjectsCollection::get().m_dynamicColliders);
		}

		static std::unordered_map<unsigned int, std::vector<Collider*>>* CollidersCollectionForRendering()
		{
			return &(GameObjectsCollection::get().m_collidersForRendering);
		}

		static std::unordered_map<unsigned int, std::vector<RigidBody*>>* RigidBodyCollection()
		{
			return &(GameObjectsCollection::get().m_rigidBodysCollection);
		}
	};

	class SystemRequests
	{
		friend class CollisionSystem;
	public:
		SystemRequests()
			: m_defaultColliderDynamic(COLLIDER_DYNAMIC::DYNAMIC)
		{}
		
		void SetDefaultColliderDynamic(COLLIDER_DYNAMIC dynamic)
		{
			GameObjectsCollection::get().m_request.m_defaultColliderDynamic = dynamic;
		}
		
		COLLIDER_DYNAMIC GetDefaultColliderDynamic()
		{
			return GameObjectsCollection::get().m_request.m_defaultColliderDynamic;
		}

		void SetObjectCollisionDynamic(GameObject* object, COLLIDER_DYNAMIC dynamic);

	private:
		COLLIDER_DYNAMIC m_defaultColliderDynamic;
	};

	CollectionProvider m_collectionProvider;
	SystemRequests m_request;

	public:
		CollectionProvider& collectionProvider() { return m_collectionProvider; }
		SystemRequests& systemRequests() { return m_request; }
};