#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider();
	~BoxCollider();
	
	bool Intersect(Collider* collider, Vector2& mtv);

	const Vector2* bounds();

private:
	void Transform(const Vector2& translation, float rotation = 0.0f);
	void SetShape();

	bool BoxBoxIntersection(BoxCollider* boxCollider, Vector2& mtv);

	Vector2 m_vertices[4];
	bool m_isInitialized;
};