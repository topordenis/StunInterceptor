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
}
