#include "CApp.h"
#include <Windows.h>
#include <memory>
#include <d3d11.h>

#include <iostream>
#include "CWindow.h"
#include "CStunIntercept.h"
#include "CSharedComm.h"
#include "CStunChecker.h"

#include <tchar.h>

CApp::CApp() {
    

}
CApp::~CApp() {

}
void CApp::Run() {

//#ifdef  _DEBUG
//    CreateConsole();
//#endif //  DEBUG
    
  


    _mSharedComm = std::make_shared<CSharedComm>();
    
    bool bIsClient = _mSharedComm->TryAsClient();

    _mStunChecker = std::make_shared<CStunChecker>();
    _mInterceptor = std::make_shared<CStunIntercept>(_mStunChecker, _mSharedComm);

    auto cristos = _mInterceptor.get();

    if (_mSharedComm->m_bServer)
         _mSharedComm->BindReceiveMessage(std::bind(&CStunIntercept::ReceiveMessage, _mInterceptor.get(), std::placeholders::_1));
    _mInterceptor->Start();
    
    if (_mSharedComm->m_bServer) /*run as server*/
    {
        OutputDebugString("Started as host server!!!");
        _mWindow = std::make_shared<CWindow>(_mSharedComm, _mStunChecker, _mInterceptor);
        _mWindow->Initialize();
        _mWindow->RunPool();
    }
    else {
        OutputDebugString("Started as client!!!");
        std::cout << "started as client " << std::endl;
        while (true) {
            Sleep(1000);
        }
    }
    

}

void CApp::CreateConsole()
{
    if (!AllocConsole()) {
        // Add some error handling here.
        // You can call GetLastError() to get more info about the error.
        return;
    }

    // std::cout, std::clog, std::cerr, std::cin
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    // std::wcout, std::wclog, std::wcerr, std::wcin
    HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
    SetStdHandle(STD_ERROR_HANDLE, hConOut);
    SetStdHandle(STD_INPUT_HANDLE, hConIn);
    std::wcout.clear();
    std::wclog.clear();
    std::wcerr.clear();
    std::wcin.clear();
}

void CApp::DetachConsole()
{
}
