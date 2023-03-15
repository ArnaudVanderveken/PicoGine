#pragma once

#include <vector>

#include "Transform.h"

class BaseComponent;

class GameObject final
{
public:
	GameObject() noexcept;
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

	// Destroy objects
	void MarkForDelete();
	[[nodiscard]] bool IsMarkedForDelete() const;

	// Parenting
	void SetParent(GameObject* parent);

	// Components
	void AddComponent(BaseComponent* component);

	template <typename ComponentType> std::enable_if_t<std::is_base_of_v<GameObject, ComponentType>, ComponentType*>
	GetComponent()
	{
		for (auto baseComp : m_pComponents)
			if (ComponentType* component = dynamic_cast<ComponentType*>(baseComp))
				return component;

		return nullptr;
	}

	// Transform
	Transform& GetWorldTransform();
	Transform& GetLocalTransform();

private:
	/* DATA MEMBERS */
	bool m_MarkedForDelete{};
	std::vector<BaseComponent*> m_pComponents{};

	std::vector<GameObject*> m_pChildren{};
	GameObject* m_pParent{};

	std::unique_ptr<Transform> m_pLocalTransform{}, m_pWorldTransform{};
	bool m_DirtyWorldTransform{ true };
	bool m_CheckForDirtyTransform{};

	/* PRIVATE METHODS */
	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);

	void UpdateWorldTransform();
	void PropagateDirtyTransform();
};
