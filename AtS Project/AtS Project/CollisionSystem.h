#pragma once
#include <unordered_map>
#include "Collider.h"
#include "COLLIDER_DYNAMIC.h"
#include "Math.h"

class CollisionSystem
{
public:
	~CollisionSystem();

	static CollisionSystem& get()
	{
		static CollisionSystem collisionSys;
		return collisionSys;
	}

	void Init(const int* pCurrentSceneId);
	void Update();

	void SetDefaultColliderDynamicMode(COLLIDER_DYNAMIC dynamic);

	void SetDebugMode(bool debugMode);
	bool OnDebugMode();

private:
	CollisionSystem();

	COLLIDER_DYNAMIC m_defaultColliderDynamicMode;
	bool m_debugMode;

	const int* m_pCurrentSceneId;
};