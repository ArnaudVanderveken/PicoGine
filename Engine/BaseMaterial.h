#pragma once

class BaseMaterial
{
public:
	BaseMaterial() noexcept = default;
	virtual ~BaseMaterial() = default;

	BaseMaterial(const BaseMaterial& other) noexcept = delete;
	BaseMaterial& operator=(const BaseMaterial& other) noexcept = delete;
	BaseMaterial(BaseMaterial&& other) noexcept = delete;
	BaseMaterial& operator=(BaseMaterial&& other) noexcept = delete;

	virtual void Bind();

private:
	/* DATA MEMBERS */

	/* PRIVATE METHODS */
	
};