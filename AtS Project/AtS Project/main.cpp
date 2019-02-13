#include "stdafx.h"
#include "WindowSystem.h"
#include "ResourceSystem.h"
#include "ViewSystem.h"
#include "RenderSystem.h"
#include "SceneSystem.h"
#include "InputSystem.h"
#include "AnimationSystem.h"
#include "GameObjectsCollection.h"
#include "GameWorld.h"
#include "Time.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		HRESULT hr = S_OK;

		WindowSystem* wndSys = &WindowSystem::get();
		ResourceSystem* resSys = &ResourceSystem::get();
		ViewSystem* vwSys = &ViewSystem::get();
		SceneSystem* scnSys = &SceneSystem::get();
		InputSystem* inputSys = &InputSystem::get();
		RenderSystem* renderSys = &RenderSystem::get();
		AnimationSystem* animSys = &AnimationSystem::get();

		GameObjectsCollection* goc = &GameObjectsCollection::get();

		GameWorld gameWorld;


		hr = wndSys->Init();
		if (!SUCCEEDED(hr)) { return 0; }

		hr = resSys->Init();
		if (!SUCCEEDED(hr)) { return 0; }

		vwSys->Init();
		wndSys->AddMainWindowMessageListener<ViewSystem>(WM_SETFOCUS, &ViewSystem::Update, vwSys);
		wndSys->AddMainWindowMessageListener<ViewSystem>(WM_KILLFOCUS, &ViewSystem::Update, vwSys);
		wndSys->AddRenderWindowMessageListener<ViewSystem>(WM_SIZE, &ViewSystem::Update, vwSys);

		scnSys->Init();

		inputSys->Init();
		wndSys->AddMainWindowMessageListener<InputSystem>(WM_KEYDOWN, &InputSystem::Update, inputSys);
		wndSys->AddMainWindowMessageListener<InputSystem>(WM_KEYUP, &InputSystem::Update, inputSys);

		goc->Init(scnSys->currentSceneId());

		renderSys->Init();
		wndSys->AddRenderWindowMessageListener<RenderSystem>(WM_PAINT, &RenderSystem::Render, renderSys);

		animSys->Init();

		//Last init
		gameWorld.Init();

		if (SUCCEEDED(hr))
		{
			Time* time = &Time::get();

			time->deltaTime = 0.01;
			time->elapsed = 0.0;
			time->lag = 0.0;

			clock_t previousTime = clock();
			clock_t currentTime;
			clock_t lagTime;

			MSG message;

			while (GetMessage(&message, NULL, 0, 0))
			{
				currentTime = clock();
				time->elapsed = (double)(currentTime - previousTime) / CLOCKS_PER_SEC;
				previousTime = currentTime;

				time->lag += time->elapsed;
				time->time += time->elapsed;

				TranslateMessage(&message);
				DispatchMessage(&message);

				goc->StateUpdate();
				animSys->Update(time->elapsed);

				lagTime = clock();

				while (time->lag >= time->deltaTime)
				{
					//resSys->Update();
					goc->Update();
					time->lag -= time->deltaTime;
				}

				lagTime = clock() - lagTime;

				animSys->Update((double)lagTime / CLOCKS_PER_SEC);

				UpdateWindow(*wndSys->pRenderHwnd());
				/*InvalidateRect(*wndSys->pRenderHwnd(), NULL, FALSE);
				UpdateWindow(*wndSys->pRenderHwnd());*/
			}
		}

	}
}