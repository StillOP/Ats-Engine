#pragma once
#include <vector>
#include <map>
#include <deque>
#include <functional>

struct Scene
{
	std::string name;
	unsigned int id;
	bool transparent;
	bool destructive;
	bool isInitialiazed;
};

class SceneSystem
{
public:
	~SceneSystem();

	void Init();

	static SceneSystem& get()
	{
		static SceneSystem sceneSys;
		return sceneSys;
	}

	template<class T>
	void AddScene(std::string name, void(T::*init)(), T instance, bool transparent = false, bool destructive = true)
	{
		if (m_scenesId.find(name) != m_scenesId.end()) { return; }

		bool destructive_correction = transparent ? false : destructive;

		Scene scene = { name, m_nextSceneId, transparent, destructive_correction, false };
		auto init_func = std::bind(init, instance);

		m_scenes.emplace(m_nextSceneId, scene);
		m_scenesId.emplace(name, m_nextSceneId);
		m_scenesInitializers.emplace(m_nextSceneId, init_func);

		m_nextSceneId++;
	}


	void UpdateSceneInfo(std::string name, std::string newName, bool transparent = false, bool destructive = true);
	void SwitchTo(std::string name);

	const Scene* currentScene();
	const Scene* sceneInfo(std::string& name);
	const Scene* sceneInfo(unsigned int id);

	const std::deque<int>* scenesOrder();
	const int* currentSceneId();
	int sceneCount();

private:
	SceneSystem();
	
	std::map<unsigned int, Scene> m_scenes;
	std::map<std::string, unsigned int> m_scenesId;
	std::map<unsigned int, std::function<void()>> m_scenesInitializers;

	std::deque<int> m_scenesOrder;

	int m_currentScene;
	unsigned int m_nextSceneId;
};