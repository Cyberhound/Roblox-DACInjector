#include "Injection.h"

#ifdef UNICODE
#define STRING std::wstring
#define CSTRING const wchar_t *
#define CHAR wchar_t
#else
#define STRING std::string
#define CSTRING const char *
#define CHAR char
#endif

typedef DACInjectionResult(__cdecl *Inject_T)(CSTRING dllPath, BOOL bWaitForRoblox);
Inject_T pInject = NULL;

HMODULE pLibDll;


DACInjectionResult DACInjector::Inject(STRING dllPath, bool waitForRoblox)
{
	if (pInject)
		return pInject(dllPath.c_str(), waitForRoblox);
	return DACInjectionResult::LIB_NOT_LOADED;
}


bool DACInjector::Load(STRING libPath)
{
	if (!pLibDll)
	{
		if (libPath == "")
		{
			CHAR path[MAX_PATH];
			GetFullPathName(TEXT("DACInjector.dll"), MAX_PATH, path, NULL);
			libPath = path;
			printf("PATH: %s\n", path);
		}

		pLibDll = LoadLibrary(libPath.c_str());
		if (!pLibDll)
			return false;
	}

	/* Load "Inject" */
	DWORD pFunc = (DWORD)GetProcAddress(pLibDll, "Inject");
	if (!pFunc)
		return false;

	pInject = (Inject_T)pFunc;
	return true;
}

void DACInjector::Unload()
{
	if (pLibDll)
		FreeLibrary(pLibDll);
	pInject = NULL;
}