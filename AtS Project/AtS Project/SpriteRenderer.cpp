#include "stdafx.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

SpriteRenderer::SpriteRenderer()
	: m_pbitmap{ NULL }, m_opacity{ 1.0f }, m_scale{ 1.0f }
{}

SpriteRenderer::~SpriteRenderer() { m_pbitmap = NULL; }

void SpriteRenderer::bitmap(ID2D1Bitmap** bitmap)
{
	if (bitmap == nullptr) { return; }
	m_pbitmap = bitmap;

	m_size = Vector2((*bitmap)->GetSize().width, (*bitmap)->GetSize().height);

	Vector2 position = m_owner->transform.position;
	m_sourceRect = Rect(0.0f, 0.0f, m_size.x, m_size.y);
}

void SpriteRenderer::bitmap(ID2D1Bitmap** bitmap, Rect& sourceRect)
{
	if (bitmap == nullptr) { return; }
	m_pbitmap = bitmap;

	m_size = Vector2((*bitmap)->GetSize().width, (*bitmap)->GetSize().height);

	Vector2 position = m_owner->transform.position;
	m_sourceRect = sourceRect;
}

void SpriteRenderer::bitmap(ID2D1Bitmap** bitmap, Rect&& sourceRect)
{
	if (bitmap == nullptr) { return; }
	m_pbitmap = bitmap;

	m_size = Vector2((*bitmap)->GetSize().width, (*bitmap)->GetSize().height);

	Vector2 position = m_owner->transform.position;
	m_sourceRect = sourceRect;
}

ID2D1Bitmap* SpriteRenderer::bitmap() { return m_pbitmap != NULL ? *m_pbitmap : NULL; }

Rect& SpriteRenderer::sourceRect() { return m_sourceRect; }

float& SpriteRenderer::opacity() { return m_opacity; }

float& SpriteRenderer::scale() { return m_scale; }

Vector2 SpriteRenderer::size() { return m_size; }
