#pragma once

#include <vector>

class GameObject;

class GameScene
{
public:
	GameScene() noexcept = default;
	virtual ~GameScene();

	GameScene(const GameScene& other) noexcept = delete;
	GameScene& operator=(const GameScene& other) noexcept = delete;
	GameScene(GameScene&& other) noexcept = delete;
	GameScene& operator=(GameScene&& other) noexcept = delete;

	virtual void Init() = 0;
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void Render() const;

	void AddGameObject(GameObject* gameObject);

private:
	/* DATA MEMBERS */

	std::vector<GameObject*> m_Objects;
	std::vector<GameObject*> m_TrashBin;

	/* PRIVATE METHODS */
	
};

