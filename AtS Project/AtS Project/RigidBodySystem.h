#pragma once

class RigidBodySystem
{
public:
	~RigidBodySystem();

	static RigidBodySystem& get()
	{
		static RigidBodySystem rigidBodySys;
		return rigidBodySys;
	}

	void Init(const int* pCurrentSceneId);
	void Update(float dt);

private:
	RigidBodySystem();

	const int* m_pCurrentSceneId;
};