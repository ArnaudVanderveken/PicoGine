#pragma once

#include "Singleton.h"

class Renderer final : public Singleton<Renderer>
{
public:
	class RendererImpl;

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

protected:

private:
	friend class Singleton<Renderer>;
	Renderer() noexcept = default;

	/* DATA MEMBERS */

	RendererImpl* m_pRendererImpl{};

	/* PRIVATE METHODS */
	
};