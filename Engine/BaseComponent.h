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
	
	virtual void FixedUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;

	[[nodiscard]] GameObject* GetOwner() const;

	[[nodiscard]] bool IsActive() const;
	void SetActive(bool active);

	void TransformHasChanged();

protected:
	bool m_TransformHasChanged{};
	
private:
	/* DATA MEMBERS */

	GameObject* m_pOwner{};
	bool m_IsActive{ true };
	

	/* PRIVATE METHODS */

	void SetOwner(GameObject* owner);

};