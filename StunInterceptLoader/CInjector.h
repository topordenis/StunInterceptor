#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <vector>
#include <optional>

class CInjectResult {
public:
	std::string msg;
	bool status{ false };
};
class CInjector
{
public:
	CInjectResult Inject(const char* procName, const char* dllName);

	CInjectResult Inject(DWORD processID, const char* relativeDllName);

	std::optional<std::vector<DWORD>> GetTargetThreadIDFromProcName(const char* ProcName);

};