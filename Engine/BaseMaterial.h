#pragma once

class BaseMaterial
{
public:
	BaseMaterial() noexcept = default;
	~BaseMaterial() = default;

	BaseMaterial(const BaseMaterial& other) noexcept = delete;
	BaseMaterial& operator=(const BaseMaterial& other) noexcept = delete;
	BaseMaterial(BaseMaterial&& other) noexcept = delete;
	BaseMaterial& operator=(BaseMaterial&& other) noexcept = delete;

private:
	/* DATA MEMBERS */

	/* PRIVATE METHODS */
	
};