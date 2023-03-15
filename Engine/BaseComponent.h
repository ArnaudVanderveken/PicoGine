#pragma once

class BaseComponent
{
public:
	BaseComponent() noexcept = default;
	virtual ~BaseComponent() = default;

	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) noexcept = delete;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) noexcept = delete;

	void Init();
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void Render();
	
private:
	/* DATA MEMBERS */

	/* PRIVATE METHODS */

};