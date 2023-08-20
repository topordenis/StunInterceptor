#include "CLoader.h"
#include <Windows.h>
#include <memory>
#include <d3d11.h>

#include "CWindow.h"
#include "../Shared/ImGui/imgui_impl_dx11.h"
#include "../Shared/ImGui/imgui_impl_win32.h"
#include <iostream>
void CLoader::Run() {
	m_Window = std::make_shared<CWindow>();
	m_Window->Initialize();
	m_Window->RunPool();
}