#pragma once

#include "GameSettings.h"
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

	void RenderTestTriangle();

private:
	class RendererImpl;
	class DirectX11;
	/* DATA MEMBERS */
	const RendererImpl* m_pRendererImpl{};

	inline static float m_DefaultBackgroundColor[4] = { .5f, .5f, .5f, 1.0f };
	inline static bool m_VSyncEnabled{ GameSettings::useVSync };

	/* PRIVATE METHODS */
	
};