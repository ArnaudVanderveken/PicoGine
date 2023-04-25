#pragma once

#include "Singleton.h"

class MaterialManager : public Singleton<MaterialManager>
{
public:
	~MaterialManager() override = default;

	MaterialManager(const MaterialManager& other) = delete;
	MaterialManager& operator=(const MaterialManager& other) noexcept = delete;
	MaterialManager(MaterialManager&& other) = delete;
	MaterialManager& operator=(MaterialManager&& other) noexcept = delete;

	template <typename MaterialType>
	UINT32 CreateMaterial()
	{
		return 0;
	}

protected:
	MaterialManager() noexcept = default;

private:
	/* DATA MEMBERS */

	/* PRIVATE METHODS */
	
};