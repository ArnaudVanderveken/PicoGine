#include "CameraComponent.h"

#include "GameObject.h"
#include "GameSettings.h"

using namespace DirectX;

CameraComponent::CameraComponent() noexcept
	: m_Size{ 25.f }
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
	GetOwner()->RegisterNotifyDirtyTransform(this);

	m_TransformHasChanged = true; // Will recompute all matrices on the first update
}

void CameraComponent::FixedUpdate()
{
}

void CameraComponent::Update()
{
	if (m_TransformHasChanged)
	{
		m_TransformHasChanged = false;

		XMMATRIX projection{};

		if (m_PerspectiveProjection)
		{
			projection = XMMatrixPerspectiveFovLH(GameSettings::fov, GameSettings::aspectRatio, GameSettings::nearPlane, GameSettings::farPlane);
		}
		else
		{
			const float viewWidth = (m_Size > 0) ? m_Size * GameSettings::aspectRatio : GameSettings::windowWidth;
			const float viewHeight = (m_Size > 0) ? m_Size : GameSettings::windowHeight;
			projection = XMMatrixOrthographicLH(viewWidth, viewHeight, GameSettings::nearPlane, GameSettings::farPlane);
		}

		const XMVECTOR worldPosition = XMLoadFloat3(&GetOwner()->GetWorldTransform().GetPosition());
		const XMVECTOR lookAt = XMLoadFloat3(&GetOwner()->GetWorldTransform().GetForward());
		const XMVECTOR upVec = XMLoadFloat3(&GetOwner()->GetWorldTransform().GetUp());

		const XMMATRIX view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
		const XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
		const XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

		XMStoreFloat4x4(&m_Projection, projection);
		XMStoreFloat4x4(&m_View, view);
		XMStoreFloat4x4(&m_ViewInverse, viewInv);
		XMStoreFloat4x4(&m_ViewProjection, view * projection);
		XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
	}
}

void CameraComponent::LateUpdate()
{
}

void CameraComponent::Render()
{
}

const DirectX::XMFLOAT4X4& CameraComponent::GetView() const
{
	return m_View;
}

const DirectX::XMFLOAT4X4& CameraComponent::GetProjection() const
{
	return m_Projection;
}

const DirectX::XMFLOAT4X4& CameraComponent::GetViewProjection() const
{
	return m_ViewProjection;
}

const DirectX::XMFLOAT4X4& CameraComponent::GetViewInverse() const
{
	return m_ViewInverse;
}

const DirectX::XMFLOAT4X4& CameraComponent::GetViewProjectionInverse() const
{
	return m_ViewProjectionInverse;
}
