#pragma once

#include "GameSettings.h"
#include "Singleton.h"

class Renderer final : public Singleton<Renderer>
{
public:
	class RendererImpl;

	Renderer() noexcept = default;
	~Renderer() override;

	Renderer(const Renderer& other) noexcept = delete;
	Renderer& operator=(const Renderer& other) noexcept = delete;
	Renderer(Renderer&& other) noexcept = delete;
	Renderer& operator=(Renderer&& other) noexcept = delete;

	void* GetDevice() const;
	void* GetDeviceContext() const;

	void Init();
	void BeginFrame() const;
	void EndFrame() const;

	void RenderTestTriangle() const;

private:
	/* DATA MEMBERS */

	RendererImpl* m_pRendererImpl{};

	/* PRIVATE METHODS */
	
};