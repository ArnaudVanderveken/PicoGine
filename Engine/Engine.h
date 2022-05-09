#pragma once

class Engine final
{
public:
	Engine() = default;
	~Engine() = default;

	Engine(const Engine& other) = delete;
	Engine& operator=(const Engine& other) noexcept = delete;
	Engine(Engine&& other) = delete;
	Engine& operator=(Engine&& other) noexcept = delete;

	void Run();

private:
	//DATA MEMBERS

	//PRIVATE METHODS
	
};