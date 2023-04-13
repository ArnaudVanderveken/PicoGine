#pragma once

#include "BaseMaterial.h"

class ColorMaterial final : public BaseMaterial
{
public:
	class ColorMaterialImpl;

	ColorMaterial() noexcept;
	~ColorMaterial() override;

	ColorMaterial(const ColorMaterial& other) = delete;
	ColorMaterial& operator=(const ColorMaterial& other) noexcept = delete;
	ColorMaterial(ColorMaterial&& other) = delete;
	ColorMaterial& operator=(ColorMaterial&& other) noexcept = delete;

	void Bind() override;
	void SetColor(const XMFLOAT4& color) const;
	void SetColor(float r, float g, float b, float a = 1) const;

private:
	/* DATA MEMBERS */

	ColorMaterialImpl* m_pColorMaterialImpl{};

	/* PRIVATE METHODS */
	
};