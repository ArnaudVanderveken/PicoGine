#include "SceneManager.h"


void SceneManager::Init() const
{
	for (const auto& scene : m_Scenes)
		scene->Init();
}

void SceneManager::FixedUpdate() const
{
	for (const auto& scene : m_Scenes)
		scene->FixedUpdate();
}

void SceneManager::Update() const
{
	for (const auto& scene : m_Scenes)
		scene->Update();
}

void SceneManager::LateUpdate() const
{
	for (const auto& scene : m_Scenes)
		scene->LateUpdate();
}

void SceneManager::Render() const
{
	for (const auto& scene : m_Scenes)
		scene->Render();
}
