#ifndef INJECTION_H
#define INJECTION_H
#include <Windows.h>
#include <string>

#ifdef UNICODE
#define STRING std::wstring
#else
#define STRING std::string
#endif

enum DACInjectionResult
{
	//Injection has succeeded
	INJECTION_SUCCESS,
	//Injection has failed
	INJECTION_FALIURE,

	//Unable to locate DLL from DLL path given
	DLL_NOT_FOUND,
	//Unable to find Roblox's process
	ROBLOX_NOT_FOUND,

	//Library was not loaded. Call Injection.Load();
	LIB_NOT_LOADED
};

namespace DACInjector
{
	/*
	Injects a DLL into Roblox with DAC bypass.

	dllPath - Name of dll, or full path to DLL.
	waitForRoblox - If true, it will wait for Roblox's window to show up and then continue.
				 If false, it will not wait for Roblox's window and will return RobloxNotFound if Roblox was not found.
	*/
	DACInjectionResult Inject(STRING dllPath, bool waitForRoblox);

	/*
	Loads the Injector library.
	
	libPath - Path to DACInjector.dll (or your own name, for my Library dll).
	*/
	bool Load(STRING libPath = TEXT(""));
	/*
	Unloads the Injector library.
	*/
	void Unload();
}

#undef STRING //header define only

#endif // INJECTION_H