#include "stdafx.h"
#include "RenderSystem.h"
#include "WindowSystem.h"
#include "ViewSystem.h"
#include "CollisionSystem.h"
#include "GameObject.h"
#include "Time.h"
#include "Math.h"
#include "Utilities.h"
#include "BoxCollider.h"
#include <assert.h>

RenderSystem::RenderSystem() {}

RenderSystem::~RenderSystem() {}

void RenderSystem::Init() {}

void RenderSystem::Render(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowSystem* wndSys = &WindowSystem::get();
	ViewSystem* vwSys = &ViewSystem::get();
	SceneSystem* scnSys = &SceneSystem::get();
	CollisionSystem* colSys = &CollisionSystem::get();

	HRESULT hr = wndSys->CreateDeviceResource();

	auto pSpriteRendererCollection = GameObjectsCollection::get().collectionProvider().SpriteRendererCollection();
	auto pColliderCollection = GameObjectsCollection::get().collectionProvider().CollidersCollectionForRendering()[(*scnSys->currentSceneId())];

	if (pSpriteRendererCollection->empty() && pColliderCollection.empty()) { return; }

	auto scenes = scnSys->scenesOrder();

	RECT viewRect;
	RECT intersectionRect;

	Rect mViewRect = vwSys->viewRect();

	SetRect(&viewRect, mViewRect.left, mViewRect.top, mViewRect.right, mViewRect.bottom);

	if (SUCCEEDED(hr))
	{
		ID2D1HwndRenderTarget* prenderTarget = wndSys->pRenderTarget();

		prenderTarget->BeginDraw();
		prenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		prenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_POINT_2F scaleOrigin;
		scaleOrigin.x = vwSys->scaleOrigin().x;
		scaleOrigin.y = vwSys->scaleOrigin().y;

		prenderTarget->SetTransform(
			D2D1::Matrix3x2F::Scale(vwSys->scale().x, vwSys->scale().y, scaleOrigin)
			*
			D2D1::Matrix3x2F::Translation(vwSys->translation().x, vwSys->translation().y)
		);

		if (colSys->OnDebugMode())
		{
			HRESULT hr;

			for (auto& collectionIt : pColliderCollection)
			{
				for (auto& pColliderIt : collectionIt.second)
				{
					ID2D1GeometrySink *pSink = nullptr;
					ID2D1PathGeometry* geometry = nullptr;
					ID2D1SolidColorBrush* brush = nullptr;

					hr = wndSys->pFactory()->CreatePathGeometry(&geometry);

					if (SUCCEEDED(hr))
					{
						if (pColliderIt->type() == COLLIDER_TYPE::BOXCOLLIDER)
						{
							hr = geometry->Open(&pSink);

							if (SUCCEEDED(hr))
							{
								BoxCollider* pBoxCollider = dynamic_cast<BoxCollider*>(pColliderIt);

								const Vector2* vertices = pBoxCollider->bounds();

								pSink->BeginFigure(
									D2D1::Point2F(vertices[0].x, vertices[0].y),
									D2D1_FIGURE_BEGIN_FILLED
								);

								pSink->AddLine(D2D1::Point2F(vertices[1].x, vertices[1].y));
								pSink->AddLine(D2D1::Point2F(vertices[2].x, vertices[2].y));
								pSink->AddLine(D2D1::Point2F(vertices[3].x, vertices[3].y));
								pSink->AddLine(D2D1::Point2F(vertices[0].x, vertices[0].y));

								pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

								hr = pSink->Close();
							}

							Utilities::get().SafeRelease(&pSink);

							hr = prenderTarget->CreateSolidColorBrush(
								D2D1::ColorF(D2D1::ColorF::Red),
								&brush
							);

							if (SUCCEEDED(hr))
							{
								prenderTarget->DrawGeometry(geometry, brush, 2.0f);
							}
						}
					}
				}
			}
		}

		if (!pSpriteRendererCollection->empty())
		{
			for (auto scene = scenes->rbegin(); scene != scenes->rend(); ++scene)
			{
				for (auto& layer : pSpriteRendererCollection->at(*scene))
				{
					for (auto& spriteRenderer : layer.second)
					{
						if (spriteRenderer->owner()->HasComponent<RigidBody>())
						{
							//Deal with lag here
							continue;
						}
						else
						{
							ID2D1Bitmap* bitmap = spriteRenderer->bitmap();

							if (!bitmap) { continue; }

							Vector2 position = spriteRenderer->owner()->transform.position();

							Rect sourceRect = spriteRenderer->sourceRect();
							RECT destinationRect;
							SetRect(
								&destinationRect,
								position.x,
								position.y,
								position.x + (sourceRect.right - sourceRect.left) * spriteRenderer->scale(),
								position.y + (sourceRect.bottom - sourceRect.top) * spriteRenderer->scale()
							);

							if (!IntersectRect(&intersectionRect, &viewRect, &destinationRect)) { continue; }

							prenderTarget->DrawBitmap(
								bitmap,
								D2D1::RectF(
									destinationRect.left,
									destinationRect.top,
									destinationRect.right,
									destinationRect.bottom
								),
								spriteRenderer->opacity(),
								D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
								D2D1::RectF(
									sourceRect.left,
									sourceRect.top,
									sourceRect.right,
									sourceRect.bottom
								)
							);
						}
					}
				}

				if (!scnSys->sceneInfo(*scene)->transparent) { break; }
			}
		}
		hr = prenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		wndSys->DiscardDeviceResource();
		hr = S_OK;
	}
}