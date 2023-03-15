#pragma once

#include <DirectXMath.h>


class Transform final
{
public:
	Transform() noexcept = default;
	~Transform() = default;

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
	 * \brief Set position vector
	 * \param x Position.x
	 * \param y Position.y
	 * \param z Position.z
	 */
	void SetPosition(float x, float y, float z);
	/**
	 * \brief Set position vector
	 * \param position Position vector as XMFLOAT3
	 */
	void SetPosition(const DirectX::XMFLOAT3& position);
	/**
	 * \brief Set position vector
	 * \param position Position vector as XMVECTOR
	 */
	void SetPosition(const DirectX::XMVECTOR& position);

	/**
	 * \brief Set rotation quaternion from Euler angles
	 * \param x Angle around x-axis
	 * \param y Angle around y-axis
	 * \param z Angle around z-axis
	 * \param isDegree Toggle between degrees and radians, degrees by default
	 */
	void SetRotation(float x, float y, float z, bool isDegree = true);
	/**
	 * \brief Set rotation quaternion from Euler angles
	 * \param rotation Euler angles around x- y- and z-axis
	 * \param isDegree Toggle between degrees and radians, degrees by default
	 */
	void SetRotation(const DirectX::XMFLOAT3& rotation, bool isDegree = true);
	/**
	 * \brief Set rotation quaternion
	 * \param rotation Rotation quaternion as XMFLOAT4
	 */
	void SetRotation(const DirectX::XMFLOAT4& rotation);
	/**
	 * \brief Set rotation quaternion
	 * \param rotation Rotation quaternion as XMVECTOR
	 */
	void SetRotation(const DirectX::XMVECTOR& rotation);

	/**
	 * \brief Set scale vector
	 * \param x Scale.x
	 * \param y Scale.y
	 * \param z Scale.z
	 */
	void SetScale(float x, float y, float z);
	/**
	 * \brief Set uniform scale
	 * \param s Scale
	 */
	void SetScale(float s);
	/**
	 * \brief Set scale vector
	 * \param scale Scale vector as XMFLOAT3
	 */
	void SetScale(const DirectX::XMFLOAT3& scale);

private:
	/* DATA MEMBERS */
	DirectX::XMFLOAT3 m_Position{};
	DirectX::XMFLOAT4 m_Rotation{};
	DirectX::XMFLOAT3 m_Scale{};

	DirectX::XMFLOAT4X4 m_Transform{};

	bool m_DirtyTransform{};

	/* PRIVATE METHODS */
	void RebuildTransform();
};