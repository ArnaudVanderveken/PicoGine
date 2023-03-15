#pragma once

#include <vector>

class BaseComponent;

class GameObject final
{
public:
	GameObject() noexcept = default;
	~GameObject();

	GameObject(const GameObject& other) noexcept = delete;
	GameObject& operator=(const GameObject& other) noexcept = delete;
	GameObject(GameObject&& other) noexcept = delete;
	GameObject& operator=(GameObject&& other) noexcept = delete;

	void Init();
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void Render();

	void MarkForDelete();
	[[nodiscard]] bool IsMarkedForDelete() const;

	void AddComponent(BaseComponent* component);

	template <typename ComponentType> std::enable_if_t<std::is_base_of_v<GameObject, ComponentType>, ComponentType*>
	GetComponent()
	{
		for (auto baseComp : m_pComponents)
			if (ComponentType* component = dynamic_cast<ComponentType*>(baseComp))
				return component;

		return nullptr;
	}

private:
	/* DATA MEMBERS */
	bool m_MarkedForDelete{};
	std::vector<BaseComponent*> m_pComponents{};

	std::vector<GameObject*> m_pChildren{};
	GameObject* m_pParent{};

	/* PRIVATE METHODS */
	
};