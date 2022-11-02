#pragma once

class Renderer final
{
public:
	Renderer() noexcept = default;
	~Renderer() = default;

	Renderer(const Renderer& other) noexcept = delete;
	Renderer& operator=(const Renderer& other) noexcept = delete;
	Renderer(Renderer&& other) noexcept = delete;
	Renderer& operator=(Renderer&& other) noexcept = delete;

private:
	/* DATA MEMBERS */

	/* PRIVATE METHODS */
	
};