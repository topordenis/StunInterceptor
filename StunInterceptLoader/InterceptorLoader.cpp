#include <stdio.h> 

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

#include "CInjector.h"

#include "CLoader.h"
int CALLBACK WinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	LPSTR       lpCmdLine,
	int         nCmdShow
)
{
	CLoader loader;
	loader.Run();
	//std::cout << "loading stun in this file" << std::endl;

	//std::cout << "-- WebSec Telegram Injector --" << std::endl;
	////LoadLibrary("STUNIntercept.dll");
	////Sleep(1000);
	//CInjector injector;
	//if (injector.Inject("Telegram.exe", "STUNIntercept.dll"))
	//	std::cout << "[+] Injected Successfully" << std::endl;

	//
	//std::cout << "-- WebSec Telegram Injector --" << std::endl;

	//while (true) {
	//	Sleep(1);
	//}
}
