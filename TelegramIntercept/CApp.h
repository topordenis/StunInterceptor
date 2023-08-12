#pragma once
#include "CWindow.h"

class CApp
{
public:
	CWindow m_Window;
	CApp();
	~CApp();
	void Run();
	void CreateConsole();
	void DetachConsole();
};

