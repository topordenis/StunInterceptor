#include "CInjector.h"
bool CInjector::Inject(const char* procName, const char* dllName)
{
	// Get the process id from the process name
	DWORD processID = GetTargetThreadIDFromProcName(procName);

	return Inject(processID, dllName);
}
bool CInjector::Inject(DWORD processID, const char* relativeDllName) {

	if (processID == 0)
	{
		return false; // tlhelp was unable to find the process name
	}

	HANDLE Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (Proc == 0)
	{
		// this process id clearly isnt valid or is 0 - bail!
		// this can also happen if we dont have the privileges required to access this process
		std::cout << "[!] OpenProcess() failed: " << GetLastError() << std::endl;
		return false;
	}
	std::cout << "[+] Telegram Process Found" << std::endl;
	char DllName[MAX_PATH];

	// get the full path to the dll - this is important as the target processes working directory is probably different !
	GetFullPathNameA(relativeDllName, MAX_PATH, DllName, NULL);


	LPVOID LoadLib = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

	// Allocate space in the process for our DLL name
	LPVOID RemoteString = VirtualAllocEx(Proc, NULL, strlen(DllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// Write the string name of our DLL in the memory allocated
	WriteProcessMemory(Proc, RemoteString, DllName, strlen(DllName), NULL);

	// Load our DLL by calling loadlibrary in the other process and passing our dll name
	CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLib, (LPVOID)RemoteString, NULL, NULL);

	CloseHandle(Proc);
	return true;
}
DWORD CInjector::GetTargetThreadIDFromProcName(const char* ProcName) {
	// create a handle to the toolhelp32 library
	HANDLE thSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (thSnapShot == INVALID_HANDLE_VALUE)
	{
		//MessageBox(NULL, "Error: Unable to create toolhelp snapshot!", "2MLoader", MB_OK);
		std::cout << "[!] Error: Unable to create toolhelp snapshot!" << std::endl;
		return 0;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	// iterate over the currently running processes to find the one whose name matches `ProcName`
	BOOL retval = Process32First(thSnapShot, &pe);
	while (retval)
	{
		if (!strcmp(pe.szExeFile, ProcName))
		{
			// names match
			// close the handle and return the process id
			CloseHandle(thSnapShot);
			return pe.th32ProcessID;
		}
		retval = Process32Next(thSnapShot, &pe);
	}

	// unable to find the process
	// close the handle and return 0 signalling that we were unable to find the process id

	std::cout << "[-] " << ProcName << " Not Found" << std::endl;
	std::cout << "[!] Please open " << ProcName << " process first." << std::endl;

	CloseHandle(thSnapShot);

	return 0;
}