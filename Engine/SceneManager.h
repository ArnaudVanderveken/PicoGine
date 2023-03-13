#pragma once

#include <vector>

#include "GameScene.h"
#include "Singleton.h"

class SceneManager final : public Singleton<SceneManager>
{
public:
	~SceneManager() override;

	SceneManager(const SceneManager& other) noexcept = delete;
	SceneManager& operator=(const SceneManager& other) noexcept = delete;
	SceneManager(SceneManager&& other) noexcept = delete;
	SceneManager& operator=(SceneManager&& other) noexcept = delete;

	void Init() const;
	void FixedUpdate() const;
	void Update() const;
	void LateUpdate() const;
	void Render() const;

	uint32_t AddScene(GameScene* scene);
	void SetActiveScene(uint32_t sceneIndex);

private:
	friend class Singleton<SceneManager>;
	SceneManager() noexcept = default;

	std::vector<GameScene*> m_Scenes;
	uint32_t m_ActiveSceneIndex{};

};