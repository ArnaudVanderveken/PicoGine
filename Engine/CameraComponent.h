#pragma once

#include "BaseComponent.h"

class CameraComponent final : public BaseComponent
{
public:
	CameraComponent() noexcept;
	~CameraComponent() override = default;

	CameraComponent(const CameraComponent& other) = delete;
	CameraComponent& operator=(const CameraComponent& other) noexcept = delete;
	CameraComponent(CameraComponent&& other) = delete;
	CameraComponent& operator=(CameraComponent&& other) noexcept = delete;
	
	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	const XMFLOAT4X4& GetView() const;
	const XMFLOAT4X4& GetProjection() const;
	const XMFLOAT4X4& GetViewProjection() const;
	const XMFLOAT4X4& GetViewInverse() const;
	const XMFLOAT4X4& GetViewProjectionInverse() const;

private:
	/* DATA MEMBERS */

	XMFLOAT4X4 m_View{};
	XMFLOAT4X4 m_Projection{};
	XMFLOAT4X4 m_ViewInverse{};
	XMFLOAT4X4 m_ViewProjection{};
	XMFLOAT4X4 m_ViewProjectionInverse{};

	float m_Size;
	bool m_PerspectiveProjection{ true };

	/* PRIVATE METHODS */

};
