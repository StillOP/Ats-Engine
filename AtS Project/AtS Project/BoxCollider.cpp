#include "stdafx.h"
#include "BoxCollider.h"
#include "GameObject.h"


BoxCollider::BoxCollider()
	: Collider(COLLIDER_TYPE::BOXCOLLIDER), m_isInitialized(false)
{}

BoxCollider::~BoxCollider()
{}

void BoxCollider::SetShape()
{
	m_isInitialized = true;

	Vector2 position = m_owner->transform.position();
	Vector2 size = m_owner->transform.size();
	float orientation = m_owner->transform.orientation();

	m_vertices[0] = Vector2(position.x - (size.x / 2), position.y - (size.y / 2.0f));
	m_vertices[1] = Vector2(m_vertices[0].x, m_vertices[0].y + size.y);
	m_vertices[2] = Vector2(m_vertices[1].x + size.x, m_vertices[1].y);
	m_vertices[3] = Vector2(m_vertices[0].x + size.x, m_vertices[0].y);
	
	Transform(Vector2(0.0f, 0.0f), orientation);
}

void BoxCollider::Transform(const Vector2& translation, float rotation)
{
	if (!m_isInitialized) { SetShape(); }

	float x, y;

	Vector2 center = m_owner->transform.position();

	for (unsigned int i = 0; i < 4; ++i)
	{
		m_vertices[i].transform(translation, rotation, center);
	}
}

bool BoxCollider::Intersect(Collider* collider, Vector2& mtv)
{
	if (!m_isInitialized) { SetShape(); }

	switch (collider->type())
	{
	case COLLIDER_TYPE::BOXCOLLIDER:
	{
		BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);

		return BoxBoxIntersection(boxCollider, mtv);

		break;
	}
	}

	return false;
}

bool BoxCollider::BoxBoxIntersection(BoxCollider* boxCollider, Vector2& mtv)
{
	Vector2 fprojection, sprojection, normal;
	float overlap, tempOverlap;
	
	auto minmaxprojection = [](const Vector2& vector, const Vector2* vertices, Vector2& projection)
	{
		projection.x = projection.y = vertices[0].dot(vector);

		for (int i = 1; i < 4; ++i)
		{
			float p = vertices[i].dot(vector);

			if (p < projection.x) { projection.x = p; }
			if (p > projection.y) { projection.y = p; }
		}
	};
	
	auto getEdge = [](const Vector2* vertices, unsigned int i) -> Vector2
	{
		int index = i % 4;

		return vertices[(index + 1) % 4] - vertices[index];
	};
	
	auto getNormal = [getEdge](const Vector2* vertices, unsigned int i) -> Vector2
	{
		Vector2 edge = getEdge(vertices, i);
	
		return edge.normal();
	};



	for (int i = 0; i < 4; ++i)
	{
		normal = getNormal(m_vertices, i);

		minmaxprojection(normal, m_vertices, fprojection);
		minmaxprojection(normal, boxCollider->bounds(), sprojection);

		if (sprojection.x > fprojection.y || sprojection.y < fprojection.x)
		{
			mtv = Vector2(0.0f, 0.0f);
			return false;
		}
		else
		{
			if (i == 0) 
			{ 
				overlap = fprojection.y - sprojection.x; 
				mtv = normal * (overlap / normal.dot(normal));

				continue; 
			}

			tempOverlap = fprojection.y - sprojection.x;

			if (tempOverlap < overlap)
			{
				overlap = tempOverlap;
				mtv = normal * (overlap / normal.dot(normal));
			}
		}
	}


	for (int i = 0; i < 4; ++i)
	{
		normal = getNormal(boxCollider->bounds(), i);

		minmaxprojection(normal, m_vertices, fprojection);
		minmaxprojection(normal, boxCollider->bounds(), sprojection);

		if (sprojection.x > fprojection.y || sprojection.y < fprojection.x)
		{
			mtv = Vector2(0.0f, 0.0f);
			return false;
		}
		else
		{
			tempOverlap = fprojection.y - sprojection.x;

			if (tempOverlap < overlap)
			{
				overlap = tempOverlap;
				mtv = normal * (overlap / normal.dot(normal));
			}
		}
	}

	return true;
}

const Vector2* BoxCollider::bounds()
{
	if (!m_isInitialized) { SetShape(); }

	return m_vertices;
}