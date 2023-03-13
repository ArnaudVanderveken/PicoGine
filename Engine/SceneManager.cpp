#include "SceneManager.h"


SceneManager::~SceneManager()
{
	for (const auto& scene : m_Scenes)
		delete scene;
}

void SceneManager::Init() const
{
	for (const auto& scene : m_Scenes)
		scene->Init();
}

void SceneManager::FixedUpdate() const
{
	if (!m_Scenes.empty())
		m_Scenes[m_ActiveSceneIndex]->FixedUpdate();
}

void SceneManager::Update() const
{
	if (!m_Scenes.empty())
		m_Scenes[m_ActiveSceneIndex]->Update();
}

void SceneManager::LateUpdate() const
{
	if (!m_Scenes.empty())
		m_Scenes[m_ActiveSceneIndex]->LateUpdate();
}

void SceneManager::Render() const
{
	if (!m_Scenes.empty())
		m_Scenes[m_ActiveSceneIndex]->Render();
}

uint32_t SceneManager::AddScene(GameScene* scene)
{
	m_Scenes.emplace_back(scene);
	return uint32_t(m_Scenes.size() - 1);
}

void SceneManager::SetActiveScene(uint32_t sceneIndex)
{
	//Prevent setting scene index to a non existing scene
	if (sceneIndex < m_Scenes.size())
		m_ActiveSceneIndex = sceneIndex;
}
