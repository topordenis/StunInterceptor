#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

class CInjector
{
public:
	bool Inject(const char* procName, const char* dllName);

	bool Inject(DWORD processID, const char* relativeDllName);

	DWORD GetTargetThreadIDFromProcName(const char* ProcName);
};