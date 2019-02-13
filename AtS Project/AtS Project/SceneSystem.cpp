#include "stdafx.h"
#include "SceneSystem.h"
#include "GameObjectsCollection.h"

SceneSystem::SceneSystem() {}

SceneSystem::~SceneSystem() {}

void SceneSystem::Init()
{
	m_currentScene = -1;
	m_nextSceneId = 0;
}


void SceneSystem::UpdateSceneInfo(std::string name, std::string newName, bool transparent, bool destructive)
{
	auto itr = m_scenesId.find(name);
	if (itr == m_scenesId.end()) { return; }

	Scene* scene = &m_scenes[itr->second];
	
	scene->name = newName;
	scene->transparent = transparent;
	scene->destructive = destructive;
}

void SceneSystem::SwitchTo(std::string name)
{
	auto itr = m_scenesId.find(name);
	if (itr == m_scenesId.end() || itr->second == m_currentScene) { return; }

	int sceneId = itr->second;

	if (m_scenesOrder.empty())
	{
		m_scenesOrder.push_back(sceneId);
	}
	else
	{
		for (auto itr = m_scenesOrder.begin(); itr != m_scenesOrder.end(); ++itr)
		{
			if ((*itr) == sceneId) { m_scenesOrder.erase(itr); break; }
		}

		m_scenesOrder.push_back(sceneId);
	}

	Scene* scene = &m_scenes[sceneId];
	if (scene->destructive && m_scenesOrder.size() > 1)
	{
		GameObjectsCollection::get().ClearScene(m_scenesOrder[m_scenesOrder.size() - 2]);
		m_scenes.erase(m_currentScene);
	}

	m_currentScene = sceneId;

	if (!scene->isInitialiazed)
	{
		m_scenesInitializers[sceneId]();
		scene->isInitialiazed = true;
	}
}

const Scene* SceneSystem::currentScene() 
{ 
	return &m_scenes[m_currentScene];
}

const Scene* SceneSystem::sceneInfo(std::string& name)
{
	auto itr = m_scenesId.find(name);
	if (itr == m_scenesId.end()) { return nullptr; }

	return &m_scenes[itr->second];
}

const Scene* SceneSystem::sceneInfo(unsigned int id)
{
	auto itr = m_scenes.find(id);
	if (itr == m_scenes.end()) { return nullptr; }

	return &m_scenes[id];
}

const std::deque<int>* SceneSystem::scenesOrder() { return &m_scenesOrder; }

const int* SceneSystem::currentSceneId() { return &m_currentScene; }

int SceneSystem::sceneCount() { return m_nextSceneId; }
