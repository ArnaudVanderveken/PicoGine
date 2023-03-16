#pragma once

#include <DirectXMath.h>

class GameObject;

class Transform
{
public:
	explicit Transform(GameObject* owner) noexcept;
	virtual ~Transform() = default;

	Transform(const Transform& other) noexcept = delete;
	Transform& operator=(const Transform& other) noexcept = delete;
	Transform(Transform&& other) noexcept = delete;
	Transform& operator=(Transform&& other) noexcept = delete;

	/**
	 * \brief 
	 * \return Position vector
	 */
	[[nodiscard]] const DirectX::XMFLOAT3& GetPosition() const;
	/**
	 * \brief 
	 * \return Rotation quaternion
	 */
	[[nodiscard]] const DirectX::XMFLOAT4& GetRotation() const;
	/**
	 * \brief 
	 * \return Scale vector
	 */
	[[nodiscard]] const DirectX::XMFLOAT3& GetScale() const;
	/**
	 * \brief 
	 * \return Transform matrix
	 */
	[[nodiscard]] const DirectX::XMFLOAT4X4& GetTransform();
	/**
	 * \brief Get dirty flag
	 * \return 
	 */
	[[nodiscard]] bool IsDirty() const;

	/**
	 * \brief Set position vector
	 * \param x Position.x
	 * \param y Position.y
	 * \param z Position.z
	 */
	virtual void SetPosition(float x, float y, float z);
	/**
	 * \brief Set position vector
	 * \param position Position vector as XMFLOAT3
	 */
	virtual void SetPosition(const DirectX::XMFLOAT3& position);
	/**
	 * \brief Set position vector
	 * \param position Position vector as XMVECTOR
	 */
	virtual void SetPosition(const DirectX::XMVECTOR& position);

	/**
	 * \brief Set rotation quaternion from Euler angles
	 * \param x Angle around x-axis
	 * \param y Angle around y-axis
	 * \param z Angle around z-axis
	 * \param isDegree Toggle between degrees and radians, degrees by default
	 */
	virtual void SetRotation(float x, float y, float z, bool isDegree = true);
	/**
	 * \brief Set rotation quaternion from Euler angles
	 * \param rotation Euler angles around x- y- and z-axis
	 * \param isDegree Toggle between degrees and radians, degrees by default
	 */
	virtual void SetRotation(const DirectX::XMFLOAT3& rotation, bool isDegree = true);
	/**
	 * \brief Set rotation quaternion
	 * \param rotation Rotation quaternion as XMFLOAT4
	 */
	virtual void SetRotation(const DirectX::XMFLOAT4& rotation);
	/**
	 * \brief Set rotation quaternion
	 * \param rotation Rotation quaternion as XMVECTOR
	 */
	virtual void SetRotation(const DirectX::XMVECTOR& rotation);

	/**
	 * \brief Set scale vector
	 * \param x Scale.x
	 * \param y Scale.y
	 * \param z Scale.z
	 */
	virtual void SetScale(float x, float y, float z);
	/**
	 * \brief Set uniform scale
	 * \param s Scale
	 */
	virtual void SetScale(float s);
	/**
	 * \brief Set scale vector
	 * \param scale Scale vector as XMFLOAT3
	 */
	virtual void SetScale(const DirectX::XMFLOAT3& scale);

	/**
	 * \brief Set transform matrix.
	 * \param transform New transform as XMFLOAT4X4
	 */
	virtual void SetTransform(const DirectX::XMFLOAT4X4& transform);
	/**
	 * \brief Set transform matrix.
	 * \param transform New transform as XMMATRIX
	 */
	virtual void SetTransform(const DirectX::XMMATRIX& transform);
	/**
	 * \brief Set transform matrix.
	 * \param position Position vector as XMFLOAT3
	 * \param rotation Rotation quaternion as XMFLOAT4
	 * \param scale Scale vector as XMFLOAT3
	 */
	virtual void SetTransform(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT3& scale);

protected:
	GameObject* m_pOwner{};

private:
	/* DATA MEMBERS */
	DirectX::XMFLOAT3 m_Position{ 0.f, 0.f, 0.f };
	DirectX::XMFLOAT4 m_Rotation{ 0.f, 0.f, 0.f, 1.f };
	DirectX::XMFLOAT3 m_Scale{ 1.f, 1.f, 1.f };

	DirectX::XMFLOAT4X4 m_Transform{};

	bool m_DirtyTransform{ true };

	/* PRIVATE METHODS */
	void RebuildTransform();
	void UnpackVectors();
};

class LocalTransform final : public Transform
{
	friend class GameObject;

public:
	explicit LocalTransform(GameObject* owner) noexcept;
	~LocalTransform() override = default;

	LocalTransform(const LocalTransform& other) = delete;
	LocalTransform& operator=(const LocalTransform& other) noexcept = delete;
	LocalTransform(LocalTransform&& other) = delete;
	LocalTransform& operator=(LocalTransform&& other) noexcept = delete;

	/**
	 * \brief Set position vector
	 * \param x Position.x
	 * \param y Position.y
	 * \param z Position.z
	 */
	void SetPosition(float x, float y, float z) override;
	/**
	 * \brief Set position vector
	 * \param position Position vector as XMFLOAT3
	 */
	void SetPosition(const DirectX::XMFLOAT3& position) override;
	/**
	 * \brief Set position vector
	 * \param position Position vector as XMVECTOR
	 */
	void SetPosition(const DirectX::XMVECTOR& position) override;

	/**
	 * \brief Set rotation quaternion from Euler angles
	 * \param x Angle around x-axis
	 * \param y Angle around y-axis
	 * \param z Angle around z-axis
	 * \param isDegree Toggle between degrees and radians, degrees by default
	 */
	void SetRotation(float x, float y, float z, bool isDegree = true) override;
	/**
	 * \brief Set rotation quaternion from Euler angles
	 * \param rotation Euler angles around x- y- and z-axis
	 * \param isDegree Toggle between degrees and radians, degrees by default
	 */
	void SetRotation(const DirectX::XMFLOAT3& rotation, bool isDegree = true) override;
	/**
	 * \brief Set rotation quaternion
	 * \param rotation Rotation quaternion as XMFLOAT4
	 */
	void SetRotation(const DirectX::XMFLOAT4& rotation) override;
	/**
	 * \brief Set rotation quaternion
	 * \param rotation Rotation quaternion as XMVECTOR
	 */
	void SetRotation(const DirectX::XMVECTOR& rotation) override;

	/**
	 * \brief Set scale vector
	 * \param x Scale.x
	 * \param y Scale.y
	 * \param z Scale.z
	 */
	void SetScale(float x, float y, float z) override;
	/**
	 * \brief Set uniform scale
	 * \param s Scale
	 */
	void SetScale(float s) override;
	/**
	 * \brief Set scale vector
	 * \param scale Scale vector as XMFLOAT3
	 */
	void SetScale(const DirectX::XMFLOAT3& scale) override;

	/**
	 * \brief Set transform matrix.
	 * \param transform New transform as XMFLOAT4X4
	 */
	void SetTransform(const DirectX::XMFLOAT4X4& transform) override;
	/**
	 * \brief Set transform matrix.
	 * \param transform New transform as XMMATRIX
	 */
	void SetTransform(const DirectX::XMMATRIX& transform) override;
	/**
	 * \brief Set transform matrix.
	 * \param position Position vector as XMFLOAT3
	 * \param rotation Rotation quaternion as XMFLOAT4
	 * \param scale Scale vector as XMFLOAT3
	 */
	void SetTransform(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT3& scale) override;

private:
	void SetWorldTransformDirty() const;
};
