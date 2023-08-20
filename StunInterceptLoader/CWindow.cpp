#include <Windows.h>
#include <memory>
#include <d3d11.h>

#include "CWindow.h"
#include "../Shared/ImGui/imgui_impl_dx11.h"
#include "../Shared/ImGui/imgui_impl_win32.h"
#include <iostream>
#include "CGui.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED) {
            ShowWindow(hWnd, 0);
            return 0;
        }
        CWindow::g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        CWindow::g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
int center_window(HWND parent_window, int width, int height, LPRECT rect)
{
    GetClientRect(parent_window, rect);
    rect->left = (rect->right / 2) - (width / 2);
    rect->top = (rect->bottom / 2) - (height / 2);
    return 0;
}

bool CWindow::RegisterWindowClass() {

    RECT rect;

    int width = 300; int height = 150;


    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);

    RECT parentRect;
    GetWindowRect(GetDesktopWindow(), &parentRect);

    auto parentWidth = abs(parentRect.right - parentRect.left);
    auto parentHeight = abs(parentRect.bottom - parentRect.top);

    parentRect.left += parentWidth / 2 - width / 2;
    parentRect.top += parentHeight / 2 - height / 2;

    windowHwnd = ::CreateWindowW(wc.lpszClassName, L"WebSec - Intercept Loader", WS_POPUPWINDOW, parentRect.left, parentRect.top, width, height, nullptr, nullptr, wc.hInstance, nullptr);
    if (windowHwnd) {
        SetWindowLong(windowHwnd, GWL_EXSTYLE,
            GetWindowLong(windowHwnd, GWL_EXSTYLE) & ~WS_EX_APPWINDOW & ~WS_EX_TOOLWINDOW);

        return true;
    }

    return true;
}
bool CWindow::CreateDeviceD3D()
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = windowHwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CWindow::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CWindow::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CWindow::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

bool CWindow::Initialize() {

    if (!RegisterWindowClass()) {
        MessageBox(0, "Failed to create the window.", "Error", MB_OK);
        return false;
    }
    // Initialize Direct3D
    if (!CreateDeviceD3D())
    {
        CleanupDeviceD3D();
        return false;
    }
    // Show the window
    ::ShowWindow(windowHwnd, SW_SHOW);
    ::UpdateWindow(windowHwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(windowHwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    mGui->InitStyle();
    return true;
}

void CWindow::Shutdown() {
    m_bRunning = false;
}
void CWindow::RunPool() {
    m_bRunning = true;
    bool m_bVisible = false;
    bool m_bStickToPraentUpdate = false;
    SetWindowLong(windowHwnd, GWL_EXSTYLE,
        GetWindowLong(windowHwnd, GWL_EXSTYLE) & ~WS_EX_APPWINDOW & ~WS_EX_TOOLWINDOW);
    while (m_bRunning)
    {

        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                std::cout << "quit " << std::endl;
                m_bRunning = false;
            }
        }
        if (!m_bRunning)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        Draggable();
      //  mGui->mWindow = windowHwnd;
        mGui->Render();


        ImGui::Render();

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());



        g_pSwapChain->Present(1, 0); // Present with vsync



    }

    mGui->Shutdown();
}
void CWindow::Draggable() {

    if (!m_bIsDragging && !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemFocused() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        m_bIsDragging = true;
        POINT pos;
        GetCursorPos(&pos);
        RECT wRect;
        GetWindowRect(windowHwnd, &wRect);

        m_RelativeRect.left = pos.x - wRect.left;
        m_RelativeRect.top = pos.y - wRect.top;

    }
    else if (m_bIsDragging && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        m_bIsDragging = false;
    }
    else if (m_bIsDragging && windowHwnd) {
        POINT pos;
        GetCursorPos(&pos);
        RECT currentRect;

        currentRect.left = pos.x - m_RelativeRect.left;
        currentRect.top = pos.y - m_RelativeRect.top;
        SetWindowPos(windowHwnd, nullptr, currentRect.left, currentRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    }
}

CWindow::CWindow()
{
    mGui = std::make_shared<CGui>();
}

CWindow::~CWindow()
{
}
