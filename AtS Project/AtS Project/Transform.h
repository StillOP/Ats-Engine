#pragma once
#include "Component.h"
#include "Math.h"


class Transform : public Component
{
public:
	Transform();
	~Transform();

	void Translate(const Vector2& vector);
	void Rotate(float angle);

	void position(const Vector2& position);
	void size(const Vector2& size);
	void orientation(float orientation);

	Vector2 position();
	Vector2 size();
	float orientation();

private:
	Vector2 m_position;
	Vector2 m_lastPosition;

	Vector2 m_size;
	Vector2 m_scale;

	float m_orientation;
	float m_lastOrientation;
};