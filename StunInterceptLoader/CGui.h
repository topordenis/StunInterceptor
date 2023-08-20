#pragma once


#include <vector>


#include "CInjector.h"
class CGui {
public:

	void Render();
	void InitImGui();
	void InitStyle();

	void Shutdown();

	std::vector< CInjectResult> m_results;
	CGui();
};