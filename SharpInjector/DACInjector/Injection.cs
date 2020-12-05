using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace DACInjector
{
	/*
	 * This is a special imports class for useful utilities in C++ to
	 * do what I know how to do in C++, since C# can't do it.
	 */
	public class Util
	{
		/* "LoadLibrary" is a function that will allow us to
		 * load the dll/library into memory.
		 */
		[DllImport("Kernel32.dll")]
		public static extern IntPtr LoadLibrary([MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

		/* "FreeLibrary" is a function that allows us to
		 * free the dll/library from memory.
		 */
		[DllImport("Kernel32.dll")]
		public static extern Boolean FreeLibrary(IntPtr hLibModule);


		/* "GetProcAddress" is a function that allows us to
		 * fetch an address with a name in it, to get a function or a procedure address.
		 */
		[DllImport("Kernel32.dll")]
		public static extern IntPtr GetProcAddress(IntPtr hModule, [MarshalAs(UnmanagedType.LPStr)] string lpProcName);
	}


	/* DACInjector Library */
	enum DACInjectionResult
	{
		//Injection has succeeded
		InjectionSuccess,
		//Injection has failed
		InjectionFailure,

		//Unable to locate DLL from DLL path given
		DllNotFound,
		//Unable to find Roblox's process
		RobloxNotFound,

		//Library was not loaded. Call Injection.Load();
		LibNotLoaded
	};

	static class Injection
	{
		/* Imports */
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		private delegate DACInjectionResult Inject_T(string dllPath, bool waitForRoblox);
		private static Inject_T pInject;

		private static IntPtr pLibDll;

		/* Wrapper */
		/*
		 * Injects a DLL with DAC bypass.
		 * 
		 * dllPath - Name of dll, or full path to DLL.
		 * waitForRoblox - If true, it will wait for Roblox's window to show up and then continue.
		 *				If false, it will not wait for Roblox's window and will return RobloxNotFound if Roblox was not found.
		 */
		public static DACInjectionResult Inject(string dllPath, bool waitForRoblox)
		{
			if (pInject != null)
				return pInject(dllPath, waitForRoblox);
			return DACInjectionResult.LibNotLoaded;
		}

		/* Loader of library */
		/*
		 * Loads the Injector library.
		 * 
		 * libPath - Path to DACInjector.dll (or your own name, for my Library dll).
		 */
		public static bool Load(string libPath = "")
		{
			if (pLibDll == IntPtr.Zero)
			{
				if (libPath == "")
					libPath = Path.GetFullPath("DACInjector.dll");

				pLibDll = Util.LoadLibrary(libPath);
				if (pLibDll == IntPtr.Zero)
					return false;
			}

			/* Load "Inject" */
			IntPtr pFunc = Util.GetProcAddress(pLibDll, "Inject");
			if (pFunc == IntPtr.Zero)
				return false;

			pInject = Marshal.GetDelegateForFunctionPointer<Inject_T>(pFunc);
			return true;
		}

		/*
		 * Unloads the Injector library.
		*/
		public static void Unload()
		{
			if (pLibDll != IntPtr.Zero)
				Util.FreeLibrary(pLibDll);
			pInject = null;
		}
	}
}
