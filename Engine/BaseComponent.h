#pragma once

class GameObject;
class BaseComponent
{
	friend class GameObject;
public:
	BaseComponent() noexcept = default;
	virtual ~BaseComponent() = default;

	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) noexcept = delete;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) noexcept = delete;

	virtual void Init() = 0;
	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

	[[nodiscard]] GameObject* GetOwner() const;

	[[nodiscard]] bool IsActive() const;
	void SetActive(bool active);
	
private:
	/* DATA MEMBERS */

	GameObject* m_pOwner{};
	bool m_IsActive{ true };

	/* PRIVATE METHODS */

	void SetOwner(GameObject* owner);

};