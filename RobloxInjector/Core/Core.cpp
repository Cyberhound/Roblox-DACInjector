#include <Windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <DACInjector/Injection.h>

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

const CHAR *resultToString(DACInjectionResult result)
{
	switch (result)
	{
		case DACInjectionResult::DLL_NOT_FOUND: return "DLL_NOT_FOUND";
		case DACInjectionResult::INJECTION_FALIURE: return "INJECTION_FALIURE";
		case DACInjectionResult::INJECTION_SUCCESS: return "INJECTION_SUCCESS";
		case DACInjectionResult::LIB_NOT_LOADED: return "LIB_NOT_LOADED";
		case DACInjectionResult::ROBLOX_NOT_FOUND: return "ROBLOX_NOT_FOUND";
		default: break;
	}
	return "(null)";
}


int main(int argc, CHAR **argv)
{
	SetConsoleTitle(TEXT("RobloxInjector - by Cyberhound#5672 [Roblox DAC Bypassed Injector]"));

	DACInjectionResult result;
	STRING dllPath = "";
	bool waitForRoblox = false;

	if (argc > 0 && argc <= 3)
	{
		if (argv[1] != "-path" || argc == 2)
		{
			dllPath = argv[1];

			if (!FileExists(dllPath))
			{
				CHAR path[MAX_PATH];
				GetFullPathName(dllPath.c_str(), MAX_PATH, path, NULL);
				dllPath = path;
			}

			if (!FileExists(dllPath))
			{
				printf("What the hell did you give me? I don't understand what '%s' is. (Exiting in 5 seconds).\n", argv[1]);
				Sleep(5000);
				return 1;
			}
		}
		else
			dllPath = argv[2];

		if (!DACInjector::Load())
		{
			printf("Failed to load library for Injector. (DACInjector.dll) Exiting in 5 seconds...\n");
			Sleep(5000);
			return 1;
		}

		if ((result = DACInjector::Inject(dllPath, waitForRoblox)) == DACInjectionResult::ROBLOX_NOT_FOUND)
		{

			printf("Do you want to wait for Roblox to open before injection (y/n)? ");
			{
				STRING input;
				std::getline(std::cin, input);
				std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return std::tolower(c); });
				waitForRoblox = input[0] == 'y' && input.find("no") == std::string::npos;

				if (input[0] == 'y' && input.size() > 2 && input[1] != 'e')
					printf("I'll just take that as a yes?\n");
			}
			printf("\n");

			result = DACInjector::Inject(dllPath, waitForRoblox);
		}

		/* unload, and give result */
		printf("Injection result: %s\n", resultToString(result));
		DACInjector::Unload();
		Sleep(5000);
		return 0;
	}

	/* No args given... Prompt user about the arguments. */
	printf("Greetings, user!\n\tHi I'm Cyberhound.\n\n");
	printf("You did not give me a path for your DLL (or too many arguments). To do so, you can drag a DLL onto the injector or put a \"-path\" argument then your DLL's path.\n");
	printf("But, I will still accept you and let you type the DLL path here.\n");

	/* Prompt for Path */
	printf("Path: ");
	std::getline(std::cin, dllPath);

	if (!DACInjector::Load())
	{
		printf("Failed to load library for Injector. (DACInjector.dll) Exiting in 5 seconds...\n");
		Sleep(5000);
		return 1;
	}

	if ((result = DACInjector::Inject(dllPath, waitForRoblox)) == DACInjectionResult::ROBLOX_NOT_FOUND)
	{

		printf("Do you want to wait for Roblox to open before injection (y/n)? ");
		{
			STRING input;
			std::getline(std::cin, input);
			std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return std::tolower(c); });
			waitForRoblox = input[0] == 'y' && input.find("no") == std::string::npos;

			if (input[0] == 'y' && input.size() > 2 && input[1] != 'e')
				printf("I'll just take that as a yes?\n");
		}
		printf("\n");

		result = DACInjector::Inject(dllPath, waitForRoblox);
	}

	/* unload, and give result */
	printf("Injection result: %s\r\nExiting in 5 seconds...", resultToString(result));
	DACInjector::Unload();

	Sleep(5000);
	return 0;
}