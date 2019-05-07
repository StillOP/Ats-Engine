#include "stdafx.h"
#include "Collider.h"
#include "GameObjectsCollection.h"


Collider::Collider(COLLIDER_TYPE type)
	: m_colliderType(type)
{}

void Collider::SetShape()
{}

void Collider::colliderDynamic(COLLIDER_DYNAMIC colliderDynamic) 
{ 
	GameObjectsCollection::get().systemRequests().SetObjectCollisionDynamic(m_owner, colliderDynamic);
	m_colliderDynamic = colliderDynamic; 
}

COLLIDER_DYNAMIC Collider::colliderDynamic() { return m_colliderDynamic; }

COLLIDER_TYPE Collider::type() { return m_colliderType; }
