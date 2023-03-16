#include "GameScene.h"

#include "GameObject.h"

GameScene::~GameScene()
{
	for (const auto& object : m_Objects)
		delete object;
}

void GameScene::FixedUpdate()
{
	for (const auto& object : m_Objects)
		object->FixedUpdate();
}

void GameScene::Update()
{
	for (const auto& object : m_Objects)
		object->Update();
}

void GameScene::LateUpdate()
{
	for (const auto& object : m_Objects)
		object->LateUpdate();

	for (const auto& object : m_Objects)
		if (object->IsMarkedForDelete())
			m_TrashBin.emplace_back(object);

	for (const auto& object : m_TrashBin)
	{
		delete object;
		std::erase(m_Objects, object);
	}
	m_TrashBin.clear();

}

void GameScene::Render()
{
	for (const auto& object : m_Objects)
		object->Render();
}

void GameScene::AddGameObject(GameObject* gameObject)
{
	m_Objects.emplace_back(gameObject);
}

