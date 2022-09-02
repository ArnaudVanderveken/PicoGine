#pragma once

#include <exception>
#include <string>

class PicoGineException : std::exception
{
public:
	PicoGineException(int line, const char* file) noexcept;
	~PicoGineException() override = default;

	PicoGineException(const PicoGineException& other) = delete;
	PicoGineException& operator=(const PicoGineException& other) noexcept = delete;
	PicoGineException(PicoGineException&& other) = delete;
	PicoGineException& operator=(PicoGineException&& other) noexcept = delete;

	[[nodiscard]] const char* what() const noexcept override;
	[[nodiscard]] virtual const char* GetType() const noexcept;
	[[nodiscard]] int GetLine() const noexcept;
	[[nodiscard]] const std::string& GetFile() const noexcept;
	[[nodiscard]] std::string GetOriginString()const noexcept;

protected:
	mutable std::string m_WhatBuffer;

private:
	/* DATA MEMBERS */
	int m_Line;
	std::string m_File;

	/* PRIVATE METHODS */
	
};

