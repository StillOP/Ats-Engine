#pragma once
#include "Component.h"
#include "COLLIDER_DYNAMIC.h"
#include "COLLIDER_TYPE.h"
#include "Math.h"


class Collider : public Component
{
	friend class Transform;
public:
	virtual ~Collider() = 0 {}

	virtual bool Intersect(Collider* collider, Vector2& mtv) = 0;

	void colliderDynamic(COLLIDER_DYNAMIC colliderDynamic);
	COLLIDER_DYNAMIC colliderDynamic();

	COLLIDER_TYPE type();

protected:
	Collider(COLLIDER_TYPE type);

	virtual void Transform(const Vector2& translation, float rotation = 0.0f) = 0;
	virtual void SetShape();

	COLLIDER_TYPE m_colliderType;
	COLLIDER_DYNAMIC m_colliderDynamic;
};