#pragma once

#include <DirectXMath.h>

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

	const DirectX::XMFLOAT4X4& GetView() const;
	const DirectX::XMFLOAT4X4& GetProjection() const;
	const DirectX::XMFLOAT4X4& GetViewProjection() const;
	const DirectX::XMFLOAT4X4& GetViewInverse() const;
	const DirectX::XMFLOAT4X4& GetViewProjectionInverse() const;

private:
	/* DATA MEMBERS */

	DirectX::XMFLOAT4X4 m_View{};
	DirectX::XMFLOAT4X4 m_Projection{};
	DirectX::XMFLOAT4X4 m_ViewInverse{};
	DirectX::XMFLOAT4X4 m_ViewProjection{};
	DirectX::XMFLOAT4X4 m_ViewProjectionInverse{};

	float m_Size;
	bool m_PerspectiveProjection{ true };

	/* PRIVATE METHODS */

};
