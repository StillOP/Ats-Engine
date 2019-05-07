#include "stdafx.h"
#include "RigidBodySystem.h"
#include "GameObjectsCollection.h"
#include "GameObject.h"
#include "RigidBody.h"


RigidBodySystem::RigidBodySystem() {}

RigidBodySystem::~RigidBodySystem() {}

void RigidBodySystem::Init(const int* pCurrentSceneId)
{
	m_pCurrentSceneId = pCurrentSceneId;
}

void RigidBodySystem::Update(float dt)
{
	auto rigidBodysCollection = GameObjectsCollection::get().collectionProvider().RigidBodyCollection();

	if (rigidBodysCollection->empty()) { return; }

	auto collectionIt = rigidBodysCollection->find(*m_pCurrentSceneId);

	if (collectionIt == rigidBodysCollection->end()) { return; }

	for (auto& rigidBody : collectionIt->second)
	{
		Vector2 nextPosition;
		Vector2 nextAcceleration;
		Vector2 nextVelocity;
		Vector2 totalForce = rigidBody->TotalForce();

		nextPosition = rigidBody->owner()->transform.position() + rigidBody->velocity() + (rigidBody->acceleration() * 0.5 * dt * dt);
		nextAcceleration = totalForce *  (1 / rigidBody->mass());
		nextVelocity = rigidBody->velocity() + ((rigidBody->acceleration() + nextAcceleration) * 0.5 * dt);

		rigidBody->acceleration(nextAcceleration);
		rigidBody->velocity(nextVelocity);
		rigidBody->position(nextPosition);

		rigidBody->SetTotalForce(Vector2(0.0f, 0.0f));
	}


}