#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <vector>
#include <mutex>

class CStunIntercept;

class CGui {
public:
public:

	HWND mWindow;
	void Render();
	void InitImGui();
	void InitStyle();

	void Shutdown();

	void DrawMemory();
	void DrawInterceptor();
	void DrawAbout();
	std::shared_ptr<CStunIntercept> intercept;
	CGui();
};