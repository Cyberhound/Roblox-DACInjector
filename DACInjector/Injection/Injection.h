#ifndef INJECTION_H
#define INJECTION_H
#include <Windows.h>
#include <Psapi.h>
#include <string>
#include <TlHelp32.h>

#ifdef UNICODE
#define STRING std::wstring
#else
#define STRING std::string
#endif

namespace Injection
{
	/*
	Injects a DLL into a process.

	dllPath - FULL path to DLL, or just the DLL name (uses parent directory [GetFullPathName]).
	hProc - Handle to the process to inject in, with permissions:
		- PROCESS_VM_WRITE
		- PROCESS_VM_OPERATION
	*/
	int InjectDLL(STRING dllPath, HANDLE hProc);
	/*
	Bypasses the DAC check in Roblox.

	hRbx - Handle to Roblox with the permission(s):
		- PROCESS_VM_WRITE
	*/
	void BypassDAC(HANDLE hRbx);
}

#undef STRING //header define only

#endif // INJECTION_H