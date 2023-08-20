#pragma once


#include <vector>
#include <mutex>

#include "CInjector.h"
class CGui {
public:

	void Render();
	void InitImGui();
	void InitStyle();

	void Shutdown();
	std::mutex m_Mtx;
	std::vector< CInjectResult> m_results;
	CGui();
};