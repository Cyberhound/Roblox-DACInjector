#include "Injection.h"
#include <fstream>

#ifdef UNICODE
#define STRING std::wstring
#define CHAR wchar_t
#else
#define STRING std::string
#define CHAR char
#endif


BOOL FileExists(STRING path)
{
	std::fstream file;
	file.open(path.c_str(), std::ios::in);
	if (file.is_open())
	{
		file.close();
		return TRUE;
	}
	file.close();
	return FALSE;
}

//UNIVERSAL
int Injection::InjectDLL(STRING dllPath, HANDLE hProc)
{
	if (hProc == INVALID_HANDLE_VALUE || !hProc)
		return 1 /* DACInjectionResult::ROBLOX_NOT_FOUND */;

	/* determine DLL path */
	if (!FileExists(dllPath))
	{
		CHAR path[MAX_PATH];
		DWORD pathSize = GetFullPathName(dllPath.c_str(), MAX_PATH, path, NULL);
		if (pathSize > 0)
			dllPath = path;
	}

	/* check if file exists */
	if (!FileExists(dllPath))
		return 2 /* DACInjectionResult::DLL_NOT_FOUND */;

	/* inject via LoadLibrary */
	//allocate dll path as a string, externally
	VOID *pPath = VirtualAllocEx(hProc, NULL, dllPath.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pPath)
		return 3 /* DACInjectionResult::INJECTION_FAILURE */;

	//write the string to the memory
	WriteProcessMemory(hProc, pPath, dllPath.c_str(), dllPath.size(), NULL);

#ifdef UNICODE
	DWORD pLoadLibrary = (DWORD)GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")), TEXT("LoadLibraryW"));
#else
	DWORD pLoadLibrary = (DWORD)GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")), TEXT("LoadLibraryA"));
#endif

	//call LoadLibrary on it
	HANDLE hDllInject = CreateRemoteThread(hProc, NULL, NULL, (LPTHREAD_START_ROUTINE)pLoadLibrary, pPath, NULL, NULL);
	WaitForSingleObject(hDllInject, INFINITE);

	//injection finished, free the dll path string
	VirtualFreeEx(hProc, pPath, NULL, MEM_RELEASE);

	//check the exit code, which gives us the result of LoadLibrary
	DWORD dwExitCode;
	GetExitCodeThread(hDllInject, &dwExitCode);

	//NULL or 0, means it failed.
	if (!dwExitCode)
		return 3 /* DACInjectionResult::INJECTION_FAILURE */;
	return 0 /* DACInjectionResult::INJECTION_SUCCESS */;
}

//ROBLOX specific
void Injection::BypassDAC(HANDLE hRbx)
{
	BYTE bypassPayload[] = { /* push ebp */ 0x55,
						/* mov ebp, esp */ 0x8B, 0xEC,
						/* jmp 5 */ 0xEB, 0x05 };

	DWORD pLdrLoadDllBypass = (DWORD)GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), TEXT("LdrLoadDll")) - 5;
	DWORD pLdrLoadDll = (DWORD)GetModuleHandle(TEXT("KERNELBASE.dll")) + 0x1CA6AC;

	WriteProcessMemory(hRbx, (LPVOID)pLdrLoadDllBypass, bypassPayload, sizeof(bypassPayload) / sizeof(BYTE), NULL);
	WriteProcessMemory(hRbx, (LPVOID)pLdrLoadDll, &pLdrLoadDllBypass, sizeof(DWORD), NULL);
}