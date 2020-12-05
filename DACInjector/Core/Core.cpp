#include <Windows.h>
#include <Injection/Injection.h>


/* Here is some basic defines that make life easier. #"cumstain: macros are bad" Lol. Ok. */
#ifdef __cplusplus //export the "C" interface version only.
#define DECLEXPORT extern "C" __declspec(dllexport)
#else
#define DECLEXPORT __declspec(dllexport)
#endif

#define EXPORT __declspec(dllexport)

#ifdef UNICODE //support UNICODE as well as Multi-Byte character sets.
#define CSTRING const wchar_t *
#else
#define CSTRING const char *
#endif

/* Here, are the wrapping exported functions for this DLL. */
EXPORT enum DACInjectionResult
{
	//Injection has succeeded
	INJECTION_SUCCESS,
	//Injection has failed
	INJECTION_FALIURE,

	//Unable to locate DLL from DLL path given
	DLL_NOT_FOUND,
	//Unable to find Roblox's process
	ROBLOX_NOT_FOUND
};


DECLEXPORT DACInjectionResult Inject(CSTRING dllPath, BOOL bWaitForRoblox)
{
	/* find Roblox's window, if that is open, it is safe to inject. */
	HWND hRbxWnd = FindWindow(NULL, TEXT("Roblox"));
	while (bWaitForRoblox && !hRbxWnd) //Wait for roblox? If so, wait until we find its window.
		hRbxWnd = FindWindow(NULL, TEXT("Roblox"));

	if (!hRbxWnd)
		return DACInjectionResult::ROBLOX_NOT_FOUND;

	/* Grab Roblox's Process ID from the Window handle. */
	DWORD pId = NULL;
	GetWindowThreadProcessId(hRbxWnd, &pId);
	if (!pId)
		return DACInjectionResult::ROBLOX_NOT_FOUND;

	/* Get access to Roblox's process for external use. */
	HANDLE hRbx = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pId);
	if (hRbx == INVALID_HANDLE_VALUE || !hRbx)
		return DACInjectionResult::ROBLOX_NOT_FOUND;

	
	/* Bypass DAC */
	Injection::BypassDAC(hRbx);

	/* Inject DLL */
	int result = Injection::InjectDLL(dllPath, hRbx);
	switch (result)
	{
		/* this should never technically happen, but whatever. won't hurt. */
		case 1: return DACInjectionResult::ROBLOX_NOT_FOUND;
		case 2: return DACInjectionResult::DLL_NOT_FOUND;
		case 3: return DACInjectionResult::INJECTION_FALIURE;
		default: break; //0, or any other number besides 1, 2, and 3.
	}

	/* We've injected successfully. :D */
	return DACInjectionResult::INJECTION_SUCCESS;
}


/* Basic DllMain setup, so we can load this via LoadLibrary. */
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpvReserved)
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(dwReasonForCall);
	UNREFERENCED_PARAMETER(lpvReserved);
	return TRUE;
}