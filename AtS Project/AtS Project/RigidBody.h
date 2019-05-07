#pragma once
#include "Transform.h"
#include "Math.h"


class RigidBody : public Component
{
public:
	RigidBody();
	~RigidBody();

	void AddForce(const Vector2& force, const Vector2& bodyCoordinates = Vector2(0.0f, 0.0f));
	//void AddTorque(const Vector2& force, const Vector2& bodyCoordinates);

	Vector2 TotalForce();
	void SetTotalForce(const Vector2& force);

	Vector2 velocity();
	Vector2 position();
	Vector2 acceleration();
	float mass();

	void velocity(const Vector2& velocity);
	void position(const Vector2& position);
	void acceleration(const Vector2& acceleration);
	void mass(float mass);

	void transform(Transform* transform);

private:
	Transform* m_transform;

	Vector2 m_forces;
	Vector2 m_acceleration;
	Vector2 m_velocity;

	Vector2 m_lastPosition;
	float m_lastOrientation;

	float m_mass;
	float m_invertMass;

	float m_speed;
};