#pragma once

#include "Singleton.h"

class Renderer final : public Singleton<Renderer>
{
public:
	Renderer() noexcept = default;
	~Renderer() override;

	Renderer(const Renderer& other) noexcept = delete;
	Renderer& operator=(const Renderer& other) noexcept = delete;
	Renderer(Renderer&& other) noexcept = delete;
	Renderer& operator=(Renderer&& other) noexcept = delete;

	void Init();
	void BeginFrame() const;
	void EndFrame() const;

	inline static constexpr float s_DefaultBackgroundColor[4] = { .5f, .5f, .5f, 1.0f };

private:
	class RendererImpl;
	class DirectX11;
	/* DATA MEMBERS */
	const RendererImpl* m_pRendererImpl{};

	/* PRIVATE METHODS */
	
};