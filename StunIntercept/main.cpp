// TelegramIntercept.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CApp.h"
#include <Windows.h>


int main()
{
    CApp app;
    app.Run();
}
unsigned long  initialize(void* instance) {
	std::cout << "create initalized \n";
	CApp app;
	app.Run();
	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	return 0;
}
std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	DisableThreadLibraryCalls(instance);



	switch (reason) {
	case DLL_PROCESS_ATTACH: {
			if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
				CloseHandle(handle);

		break;
	}

	case DLL_PROCESS_DETACH: {
		//release();
		break;
	}
	}

	return true;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
