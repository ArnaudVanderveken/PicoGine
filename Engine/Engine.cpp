#include "Engine.h"
#include "Test.h"

void Engine::Run()
{
	std::cout << "Hello world from the engine" << std::endl;
	Test test;
	test.Foo();
}
