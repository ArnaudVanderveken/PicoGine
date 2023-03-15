#pragma once


class Transform final
{
public:
	Transform() noexcept = default;
	~Transform() = default;

	Transform(const Transform& other) noexcept = delete;
	Transform& operator=(const Transform& other) noexcept = delete;
	Transform(Transform&& other) noexcept = delete;
	Transform& operator=(Transform&& other) noexcept = delete;

private:
	/* DATA MEMBERS */

	/* PRIVATE METHODS */

};