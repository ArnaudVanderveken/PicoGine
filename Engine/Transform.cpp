#include "Transform.h"

using namespace DirectX;

const XMFLOAT3& Transform::GetPosition() const
{
	return m_Position;
}

const XMFLOAT4& Transform::GetRotation() const
{
	return m_Rotation;
}

const XMFLOAT3& Transform::GetScale() const
{
	return m_Scale;
}

const XMFLOAT4X4& Transform::GetTransform()
{
	if (m_DirtyTransform)
		RebuildTransform();

	return m_Transform;
}

void Transform::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;

	m_DirtyTransform = true;
}

void Transform::SetPosition(const XMFLOAT3& position)
{
	m_Position = position;

	m_DirtyTransform = true;
}

void Transform::SetPosition(const XMVECTOR& position)
{
	XMStoreFloat3(&m_Position, position);

	m_DirtyTransform = true;
}

void Transform::SetRotation(float x, float y, float z, bool isDegree)
{
	if (isDegree)
		XMStoreFloat4(&m_Rotation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z)));
	
	else
		XMStoreFloat4(&m_Rotation, XMQuaternionRotationRollPitchYaw(x, y, z));

	m_DirtyTransform = true;	
}

void Transform::SetRotation(const XMFLOAT3& rotation, bool isDegree)
{
	SetRotation(rotation.x, rotation.y, rotation.z, isDegree);
}

void Transform::SetRotation(const XMFLOAT4& rotation)
{
	m_Rotation = rotation;

	m_DirtyTransform = true;
}

void Transform::SetRotation(const XMVECTOR& rotation)
{
	XMStoreFloat4(&m_Rotation, rotation);

	m_DirtyTransform = true;
}

void Transform::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;

	m_DirtyTransform = true;
}

void Transform::SetScale(const XMFLOAT3& scale)
{
	m_Scale = scale;

	m_DirtyTransform = true;
}

void Transform::SetScale(float s)
{
	SetScale(s, s, s);
}

void Transform::RebuildTransform()
{
	const auto world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) * XMMatrixRotationQuaternion(XMLoadFloat4(&m_Rotation)) * XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	XMStoreFloat4x4(&m_Transform, world);

	m_DirtyTransform = false;
}
