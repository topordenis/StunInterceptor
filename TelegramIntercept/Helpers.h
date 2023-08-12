#pragma once
#include <Windows.h>
#include <iostream>
namespace Helpers {
	std::uintptr_t PatternScan(const std::uintptr_t address, const char* signature, const bool relative);
}