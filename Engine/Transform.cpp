#include "Transform.h"
#include "GameObject.h"


Transform::Transform(GameObject* owner) noexcept
	: m_pOwner(owner)
{
	XMStoreFloat4x4(&m_Transform, XMMatrixIdentity());
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

const XMFLOAT3& Transform::GetForward()
{
	if (m_DirtyForward)
		RebuildForward();

	return m_Forward;
}

const XMFLOAT3& Transform::GetRight()
{
	if (m_DirtyRight)
		RebuildRight();

	return m_Right;
}

const XMFLOAT3& Transform::GetUp()
{
	if (m_DirtyUp)
		RebuildUp();

	return m_Up;
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
	m_DirtyForward = true;
	m_DirtyRight = true;
	m_DirtyUp = true;
}

void Transform::SetRotation(const XMVECTOR& rotation)
{
	XMStoreFloat4(&m_Rotation, rotation);

	m_DirtyTransform = true;
	m_DirtyForward = true;
	m_DirtyRight = true;
	m_DirtyUp = true;
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

void Transform::SetTransform(const XMFLOAT4X4& transform)
{
	m_Transform = transform;
	UnpackVectors();
}

void Transform::SetTransform(const XMMATRIX& transform)
{
	XMStoreFloat4x4(&m_Transform, transform);
}

void Transform::SetTransform(const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& scale)
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

void Transform::RebuildForward()
{
	m_DirtyForward = false;

	XMStoreFloat3(&m_Forward, XMVector3Rotate(XMLoadFloat3(&WORLD_FORWARD), XMLoadFloat4(&m_Rotation)));
}

void Transform::RebuildRight()
{
	m_DirtyRight = false;

	XMStoreFloat3(&m_Right, XMVector3Rotate(XMLoadFloat3(&WORLD_RIGHT), XMLoadFloat4(&m_Rotation)));
}

void Transform::RebuildUp()
{
	m_DirtyUp = false;

	XMStoreFloat3(&m_Up, XMVector3Rotate(XMLoadFloat3(&WORLD_UP), XMLoadFloat4(&m_Rotation)));
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

void LocalTransform::SetPosition(const XMFLOAT3& position)
{
	Transform::SetPosition(position);
	SetWorldTransformDirty();
}

void LocalTransform::SetPosition(const XMVECTOR& position)
{
	Transform::SetPosition(position);
	SetWorldTransformDirty();
}

void LocalTransform::SetRotation(float x, float y, float z, bool isDegree)
{
	Transform::SetRotation(x, y, z, isDegree);
	SetWorldTransformDirty();
}

void LocalTransform::SetRotation(const XMFLOAT3& rotation, bool isDegree)
{
	Transform::SetRotation(rotation, isDegree);
	SetWorldTransformDirty();
}

void LocalTransform::SetRotation(const XMFLOAT4& rotation)
{
	Transform::SetRotation(rotation);
	SetWorldTransformDirty();
}

void LocalTransform::SetRotation(const XMVECTOR& rotation)
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

void LocalTransform::SetScale(const XMFLOAT3& scale)
{
	Transform::SetScale(scale);
	SetWorldTransformDirty();
}

void LocalTransform::SetTransform(const XMFLOAT4X4& transform)
{
	Transform::SetTransform(transform);
	SetWorldTransformDirty();
}

void LocalTransform::SetTransform(const XMMATRIX& transform)
{
	Transform::SetTransform(transform);
	SetWorldTransformDirty();
}

void LocalTransform::SetTransform(const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& scale)
{
	Transform::SetTransform(position, rotation, scale);
	SetWorldTransformDirty();
}

void LocalTransform::SetWorldTransformDirty() const
{
	m_pOwner->PropagateDirtyTransform();
}
