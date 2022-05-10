#pragma once

class Test
{
public:
	Test() = default;
	~Test() = default;

	Test(const Test& other) = delete;
	Test& operator=(const Test& other) noexcept = delete;
	Test(Test&& other) = delete;
	Test& operator=(Test&& other) noexcept = delete;

	void TestTest();

private:
	//DATA MEMBERS

	//PRIVATE METHODS

};

