#pragma once

#include <vector>

class GameObject;

class GameScene
{
public:
	GameScene() noexcept = default;
	virtual ~GameScene() = default;

	GameScene(const GameScene& other) noexcept = delete;
	GameScene& operator=(const GameScene& other) noexcept = delete;
	GameScene(GameScene&& other) noexcept = delete;
	GameScene& operator=(GameScene&& other) noexcept = delete;

	void Init();
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void Render();

	void AddGameObject(GameObject* gameObject);

private:
	/* DATA MEMBERS */
	std::vector<GameObject*> m_Objects;

	/* PRIVATE METHODS */
	
};

