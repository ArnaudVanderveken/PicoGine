#include "Transform.h"
#include "GameObject.h"

using namespace DirectX;

Transform::Transform(GameObject* owner) noexcept
	: m_pOwner(owner)
{
}

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

bool Transform::IsDirty() const
{
	return m_DirtyTransform;
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

void Transform::SetTransform(const DirectX::XMFLOAT4X4& transform)
{
	m_Transform = transform;
	UnpackVectors();
}

void Transform::SetTransform(const DirectX::XMMATRIX& transform)
{
	XMStoreFloat4x4(&m_Transform, transform);
}

void Transform::SetTransform(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT3& scale)
{
	m_Position = position;
	m_Rotation = rotation;
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

void Transform::UnpackVectors()
{
	XMVECTOR pos, rot, scale;
	if (XMMatrixDecompose(&scale, &rot, &pos, XMLoadFloat4x4(&m_Transform)))
	{
		XMStoreFloat3(&m_Position, pos);
		XMStoreFloat4(&m_Rotation, rot);
		XMStoreFloat3(&m_Scale, scale);
	}
}

LocalTransform::LocalTransform(GameObject* owner) noexcept
	: Transform(owner)
{
}

void LocalTransform::SetPosition(float x, float y, float z)
{
	Transform::SetPosition(x, y, z);
	SetWorldTransformDirty();
}

void LocalTransform::SetPosition(const DirectX::XMFLOAT3& position)
{
	Transform::SetPosition(position);
	SetWorldTransformDirty();
}

void LocalTransform::SetPosition(const DirectX::XMVECTOR& position)
{
	Transform::SetPosition(position);
	SetWorldTransformDirty();
}

void LocalTransform::SetRotation(float x, float y, float z, bool isDegree)
{
	Transform::SetRotation(x, y, z, isDegree);
	SetWorldTransformDirty();
}

void LocalTransform::SetRotation(const DirectX::XMFLOAT3& rotation, bool isDegree)
{
	Transform::SetRotation(rotation, isDegree);
	SetWorldTransformDirty();
}

void LocalTransform::SetRotation(const DirectX::XMFLOAT4& rotation)
{
	Transform::SetRotation(rotation);
	SetWorldTransformDirty();
}

void LocalTransform::SetRotation(const DirectX::XMVECTOR& rotation)
{
	Transform::SetRotation(rotation);
	SetWorldTransformDirty();
}

void LocalTransform::SetScale(float x, float y, float z)
{
	Transform::SetScale(x, y, z);
	SetWorldTransformDirty();
}

void LocalTransform::SetScale(float s)
{
	Transform::SetScale(s);
	SetWorldTransformDirty();
}

void LocalTransform::SetScale(const DirectX::XMFLOAT3& scale)
{
	Transform::SetScale(scale);
	SetWorldTransformDirty();
}

void LocalTransform::SetTransform(const DirectX::XMFLOAT4X4& transform)
{
	Transform::SetTransform(transform);
	SetWorldTransformDirty();
}

void LocalTransform::SetTransform(const DirectX::XMMATRIX& transform)
{
	Transform::SetTransform(transform);
	SetWorldTransformDirty();
}

void LocalTransform::SetTransform(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& rotation,
	const DirectX::XMFLOAT3& scale)
{
	Transform::SetTransform(position, rotation, scale);
	SetWorldTransformDirty();
}

void LocalTransform::SetWorldTransformDirty() const
{
	m_pOwner->PropagateDirtyTransform();
}
