#pragma once
#include "GameObjectsCollection.h"
#include "GameObject.h"
#include "ViewSystem.h"
#include "Time.h"
#include "Math.h"

class CollisionTester: public Behaviour
{
public:
	CollisionTester(Haviour haviour)
		: Behaviour(haviour)
	{}

	~CollisionTester() {}
	
	void Init() 
	{
		m_rigidBody = m_owner->AddComponent<RigidBody>();
	}

	void Update()
	{
		InputSystem* inputSys = &InputSystem::get();
		double deltaTime = Time::get().deltaTime;
		
		if (inputSys->IsKeyPressed(0x25))
		{
			//m_owner->transform.Translate(Vector2(-800.0f * deltaTime, 0.0f));
			m_rigidBody->AddForce(Vector2(50.0f, 0.0f));
		}
		if (inputSys->IsKeyPressed(0x27))
		{
			m_owner->transform.Translate(Vector2(800.0f * deltaTime, 0.0f));
		}

		if (inputSys->IsKeyPressed(0x26))
		{
			m_owner->transform.Translate(Vector2(0.0f, -800.0f * deltaTime));
		}
		if (inputSys->IsKeyPressed(0x28))
		{
			m_owner->transform.Translate(Vector2(0.0f, 800.0f * deltaTime));
		}
	}

private:
	RigidBody* m_rigidBody;
};

class CollisionTestScene
{
public:
	CollisionTestScene() {}
	~CollisionTestScene() {}

	void Init()
	{
		ViewSystem* view = &ViewSystem::get();
		m_pGameObjectCollection = &GameObjectsCollection::get();

		//view->ZoomView(0.7f);

		GameObject* obj1 = m_pGameObjectCollection->gameObject<CollisionTester>("obj1");
		GameObject* obj2 = m_pGameObjectCollection->gameObject("obj2");

		obj1->transform.Translate(Vector2(100.0f, 150.0f));
		obj1->transform.size(Vector2(100.0f, 100.0f));
		obj1->transform.Rotate(Math::PI / 7);

		obj2->transform.Translate(Vector2(300.0f, 150.0f));
		obj2->transform.size(Vector2(120.0f, 120.0f));
		//obj2->transform.Rotate(Math::PI / 7);


		BoxCollider* collider1 = obj1->AddComponent<BoxCollider>();
		BoxCollider* collider2 = obj2->AddComponent<BoxCollider>();

		auto g = collider1->owner();
		g = GameObjectsCollection::get().gameObject("klm");
	}

private:
	GameObjectsCollection* m_pGameObjectCollection;
};


