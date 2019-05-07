#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"


Transform::Transform()
	:m_position(Vector2(0.0f, 0.0f)),
	m_lastPosition(Vector2(0.0f, 0.0f)),
	m_scale(Vector2(1.0f, 1.0f)),
	m_size(Vector2(1.0f, 1.0f)),
	m_orientation(0.0f),
	m_lastOrientation(0.0f)
{}

Transform::~Transform()
{}

void Transform::Translate(const Vector2& vector)
{
	Collider* pCollider = nullptr;

	m_lastPosition = m_position;
	m_position += vector;

	if (m_owner->HasComponent<BoxCollider>()) { pCollider = m_owner->GetComponent<BoxCollider>(); }

	if (pCollider == nullptr) { return; }

	pCollider->Transform(vector);
}

void Transform::Rotate(float angle)
{
	Collider* pCollider = nullptr;

	m_lastOrientation = m_orientation;
	m_orientation += angle;

	if (m_owner->HasComponent<BoxCollider>()) { pCollider = m_owner->GetComponent<BoxCollider>(); }

	if (pCollider == nullptr) { return; }

	pCollider->Transform(Vector2(0.0f, 0.0f), angle);
}

void Transform::position(const Vector2& position)
{
	Collider* pCollider = nullptr;

	m_lastPosition = m_position;
	m_position = position;

	if (m_owner->HasComponent<BoxCollider>()) { pCollider = m_owner->GetComponent<BoxCollider>(); }

	if (pCollider == nullptr) { return; }

	pCollider->SetShape();
}

void Transform::orientation(float orientation)
{
	Collider* pCollider = nullptr;

	m_lastOrientation = m_orientation;
	m_orientation = orientation;

	if (m_owner->HasComponent<BoxCollider>()) { pCollider = m_owner->GetComponent<BoxCollider>(); }

	if (pCollider == nullptr) { return; }

	pCollider->SetShape();
}

void Transform::size(const Vector2& size)
{
	Collider* pCollider = nullptr;
	
	m_size = size;

	if (m_owner->HasComponent<BoxCollider>()) { pCollider = m_owner->GetComponent<BoxCollider>(); }

	if (pCollider == nullptr) { return; }

	pCollider->SetShape();
}

Vector2 Transform::position() { return m_position; }

Vector2 Transform::size() { return m_size; }

float Transform::orientation() { return m_orientation; }