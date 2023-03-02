#pragma once

#include <vector>

#include "GameScene.h"
#include "Singleton.h"

class SceneManager final: public Singleton<SceneManager>
{
public:
	~SceneManager() override = default;

	SceneManager(const SceneManager& other) noexcept = delete;
	SceneManager& operator=(const SceneManager& other) noexcept = delete;
	SceneManager(SceneManager&& other) noexcept = delete;
	SceneManager& operator=(SceneManager&& other) noexcept = delete;

	void Init() const;
	void FixedUpdate() const;
	void Update() const;
	void LateUpdate() const;
	void Render() const;

private:
	friend class Singleton<SceneManager>;
	SceneManager() noexcept = default;

	std::vector<std::unique_ptr<GameScene>> m_Scenes;
	
};

