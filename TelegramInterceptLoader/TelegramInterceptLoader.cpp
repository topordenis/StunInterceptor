#include <stdio.h> 

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

#include "CInjector.h"


int main(int argc, char** argv)
{
	std::cout << "-- WebSec Telegram Injector --" << std::endl;

	CInjector injector;
	if (injector.Inject("Telegram.exe", "TelegramIntercept.dll"))
		std::cout << "[+] Injected Successfully" << std::endl;


	Sleep(5000);
}