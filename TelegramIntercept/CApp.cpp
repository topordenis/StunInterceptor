#include "CApp.h"
#include "CWindow.h"

CApp::CApp() {
	m_Window.Initialize();
}
CApp::~CApp() {

}
void CApp::Run() {
	CreateConsole();

	m_Window.RunPool();
	
}

void CApp::CreateConsole()
{

#if _DEBUG
	AllocConsole();

	auto hConOut = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);

	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);

	SetConsoleTitleA("Telegram console");
#endif
}

void CApp::DetachConsole()
{
}
