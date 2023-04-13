#pragma once

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
	[[nodiscard]] const XMFLOAT3& GetPosition() const;
	/**
	 * \brief 
	 * \return Rotation quaternion
	 */
	[[nodiscard]] const XMFLOAT4& GetRotation() const;
	/**
	 * \brief 
	 * \return Scale vector
	 */
	[[nodiscard]] const XMFLOAT3& GetScale() const;
	/**
	 * \brief 
	 * \return Transform matrix
	 */
	[[nodiscard]] const XMFLOAT4X4& GetTransform();
	/**
	 * \brief Get dirty flag
	 * \return 
	 */
	[[nodiscard]] bool IsDirty() const;

	/**
	 * \brief 
	 * \return Transform's forward unit vector as XMFLOAT3
	 */
	[[nodiscard]] const XMFLOAT3& GetForward();
	/**
	 * \brief 
	 * \return Transform's right unit vector as XMFLOAT3
	 */
	[[nodiscard]] const XMFLOAT3& GetRight();
	/**
	 * \brief
	 * \return Transform's up unit vector as XMFLOAT3
	 */
	[[nodiscard]] const XMFLOAT3& GetUp();

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
	virtual void SetPosition(const XMFLOAT3& position);
	/**
	 * \brief Set position vector
	 * \param position Position vector as XMVECTOR
	 */
	virtual void SetPosition(const XMVECTOR& position);

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
	virtual void SetRotation(const XMFLOAT3& rotation, bool isDegree = true);
	/**
	 * \brief Set rotation quaternion
	 * \param rotation Rotation quaternion as XMFLOAT4
	 */
	virtual void SetRotation(const XMFLOAT4& rotation);
	/**
	 * \brief Set rotation quaternion
	 * \param rotation Rotation quaternion as XMVECTOR
	 */
	virtual void SetRotation(const XMVECTOR& rotation);

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
	virtual void SetScale(const XMFLOAT3& scale);

	/**
	 * \brief Set transform matrix.
	 * \param transform New transform as XMFLOAT4X4
	 */
	virtual void SetTransform(const XMFLOAT4X4& transform);
	/**
	 * \brief Set transform matrix.
	 * \param transform New transform as XMMATRIX
	 */
	virtual void SetTransform(const XMMATRIX& transform);
	/**
	 * \brief Set transform matrix.
	 * \param position Position vector as XMFLOAT3
	 * \param rotation Rotation quaternion as XMFLOAT4
	 * \param scale Scale vector as XMFLOAT3
	 */
	virtual void SetTransform(const XMFLOAT3& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT3& scale);

	// World constants

	inline static constexpr XMFLOAT3 WORLD_FORWARD{ 0.f, 0.f, 1.f };
	inline static constexpr XMFLOAT3 WORLD_RIGHT{ 1.f, 0.f, 0.f };
	inline static constexpr XMFLOAT3 WORLD_UP{ 0.f, 1.f, 0.f };

protected:
	GameObject* m_pOwner{};

private:
	/* DATA MEMBERS */

	XMFLOAT3 m_Position{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_Rotation{ 0.f, 0.f, 0.f, 1.f };
	XMFLOAT3 m_Scale{ 1.f, 1.f, 1.f };

	XMFLOAT4X4 m_Transform;

	XMFLOAT3 m_Forward{};
	bool m_DirtyForward{};
	XMFLOAT3 m_Right{};
	bool m_DirtyRight{};
	XMFLOAT3 m_Up{};
	bool m_DirtyUp{};

	bool m_DirtyTransform{ true };

	/* PRIVATE METHODS */

	void RebuildTransform();
	void UnpackVectors();
	void RebuildForward();
	void RebuildRight();
	void RebuildUp();
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
	void SetPosition(const XMFLOAT3& position) override;
	/**
	 * \brief Set position vector
	 * \param position Position vector as XMVECTOR
	 */
	void SetPosition(const XMVECTOR& position) override;

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
	void SetRotation(const XMFLOAT3& rotation, bool isDegree = true) override;
	/**
	 * \brief Set rotation quaternion
	 * \param rotation Rotation quaternion as XMFLOAT4
	 */
	void SetRotation(const XMFLOAT4& rotation) override;
	/**
	 * \brief Set rotation quaternion
	 * \param rotation Rotation quaternion as XMVECTOR
	 */
	void SetRotation(const XMVECTOR& rotation) override;

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
	void SetScale(const XMFLOAT3& scale) override;

	/**
	 * \brief Set transform matrix.
	 * \param transform New transform as XMFLOAT4X4
	 */
	void SetTransform(const XMFLOAT4X4& transform) override;
	/**
	 * \brief Set transform matrix.
	 * \param transform New transform as XMMATRIX
	 */
	void SetTransform(const XMMATRIX& transform) override;
	/**
	 * \brief Set transform matrix.
	 * \param position Position vector as XMFLOAT3
	 * \param rotation Rotation quaternion as XMFLOAT4
	 * \param scale Scale vector as XMFLOAT3
	 */
	void SetTransform(const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& scale) override;

private:
	void SetWorldTransformDirty() const;
};
