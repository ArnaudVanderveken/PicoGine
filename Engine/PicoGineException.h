#pragma once

#include <exception>
#include <string>

class PicoGineException : public std::exception
{
public:
	PicoGineException(int line, const wchar_t* file) noexcept;
	~PicoGineException() override = default;

	PicoGineException(const PicoGineException& other) = delete;
	PicoGineException& operator=(const PicoGineException& other) noexcept = delete;
	PicoGineException(PicoGineException&& other) = delete;
	PicoGineException& operator=(PicoGineException&& other) noexcept = delete;

	[[nodiscard]] virtual const wchar_t* wwhat() const noexcept;
	[[nodiscard]] virtual const wchar_t* GetType() const noexcept;
	[[nodiscard]] int GetLine() const noexcept;
	[[nodiscard]] const std::wstring& GetFile() const noexcept;
	[[nodiscard]] std::wstring GetOriginString()const noexcept;

protected:
	mutable std::wstring m_WhatBuffer;

private:
	/* DATA MEMBERS */
	int m_Line;
	std::wstring m_File;

	/* PRIVATE METHODS */
	
};

