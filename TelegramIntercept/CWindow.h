#pragma once

#include "CGui.h"
class CWindow
{
private:
	bool RegisterWindowClass();
	bool CreateDeviceD3D();
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
private:
	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
	HWND windowHwnd = 0;
private:
	CGui mGui;
public:
	inline static UINT g_ResizeWidth = 0;
	inline static UINT g_ResizeHeight = 0;
public:
	bool Initialize();
	void RunPool();
	void Shutdown();
	void Draggable();
	bool m_bRunning = false;
	bool m_bIsDragging = false;
	RECT m_InitialRect;
	RECT m_RelativeRect;
	void CheckForResize();
	CWindow();
	~CWindow();
};

