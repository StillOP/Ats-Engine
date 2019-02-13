#pragma once
#include <d2d1.h>
#include "Component.h"
#include "Transform.h"
#include "Math.h"

class SpriteRenderer : public Component
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void bitmap(ID2D1Bitmap** bitmap);
	void bitmap(ID2D1Bitmap** bitmap, Rect& sourceRect);
	void bitmap(ID2D1Bitmap** bitmap, Rect&& sourceRect);
	ID2D1Bitmap* bitmap();

	Rect& sourceRect();

	float& opacity();
	float& scale();

	Vector2 size();

private:
	ID2D1Bitmap** m_pbitmap;

	Rect m_sourceRect;
	Vector2 m_size;

	float m_opacity;
	float m_scale;
};