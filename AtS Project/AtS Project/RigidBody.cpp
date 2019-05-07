#include "stdafx.h"
#include "RigidBody.h"

RigidBody::RigidBody()
	: m_forces(Vector2(0.0f, 0.0f)), m_acceleration(Vector2(0.0f, 0.0f)),
	m_velocity(Vector2(0.0f, 0.0f)), m_speed(0.0f), m_mass(1.0f)
{
	m_invertMass = 1 / m_mass;
}

RigidBody::~RigidBody()
{}

void RigidBody::AddForce(const Vector2& force, const Vector2& bodyCoordinates)
{
	m_forces += force;
}

void RigidBody::SetTotalForce(const Vector2& force)
{
	m_forces = force;
}

Vector2 RigidBody::TotalForce() { return m_forces; }

Vector2 RigidBody::acceleration() { return m_acceleration; }

Vector2 RigidBody::velocity() { return m_velocity; }

Vector2 RigidBody::position() { return m_transform->position(); }

float RigidBody::mass() { return m_mass; }


void RigidBody::acceleration(const Vector2& acceleration) { m_acceleration = acceleration; }

void RigidBody::velocity(const Vector2& velocity) { m_velocity = velocity; }

void RigidBody::position(const Vector2& position) { m_transform->position(position); }

void RigidBody::mass(float mass) 
{
	if (mass <= 0.0f) { return; }

	m_mass = mass;
	m_invertMass = 1 / mass;
}



void RigidBody::transform(Transform* transform) 
{ 
	m_transform = transform;

	m_lastPosition = m_transform->position();
	m_lastOrientation = m_transform->orientation();
}