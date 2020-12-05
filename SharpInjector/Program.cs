using DACInjector;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SharpInjector
{
	class Program
	{
		static void Main(string[] args)
		{
			Console.Title = "SharpInjector - by Cyberhound#5672 [Roblox DAC Bypassed Injector]";

			DACInjectionResult result;
			string dllPath = "";
			bool waitForRoblox = false;

			if (args.Length > 0 && args.Length <= 2)
			{
				if (args[0] != "-path")
				{
					dllPath = args[0];

					if (!File.Exists(dllPath))
						dllPath = Path.GetFullPath(dllPath);

					if (!File.Exists(dllPath))
					{
						Console.WriteLine("What the hell did you give me? I don't understand what {0} is. (Exiting in 5 seconds).", args[1]);
						Thread.Sleep(5000);
						return;
					}
				}
				else
					dllPath = args[1];

				if (!Injection.Load())
				{
					Console.WriteLine("Failed to load library for Injector. (DACInjector.dll) Exiting in 5 seconds...");
					Thread.Sleep(5000);
					return;
				}

				if ((result = Injection.Inject(dllPath, waitForRoblox)) == DACInjectionResult.RobloxNotFound)
				{

					Console.Write("Do you want to wait for Roblox to open before injection (y/n)? ");
					{
						string input = Console.ReadLine().ToLower();
						waitForRoblox = input[0] == 'y' && !input.Contains("no");

						if (input[0] == 'y' && input.Length > 2 && input[1] != 'e')
							Console.WriteLine("I'll just take that as a yes?");
					}
					Console.WriteLine();

					result = Injection.Inject(dllPath, waitForRoblox);
				}

				/* unload, and give result */
				Console.WriteLine("Injection result: {0}", result.ToString());
				Injection.Unload();
				return;
			}

			/* No args given... Prompt user about the arguments. */
			Console.WriteLine("Greetings, user!\r\n\tHi I'm Cyberhound.\r\n");
			Console.WriteLine("You did not give me a path for your DLL (or too many arguments). To do so, you can drag a DLL onto the injector or put a \"-path\" argument then your DLL's path.");
			Console.WriteLine("But, I will still accept you and let you type the DLL path here.");

			/* Prompt for Path */
			Console.Write("Path: ");
			dllPath = Console.ReadLine();

			if (!Injection.Load())
			{
				Console.WriteLine("Failed to load library for Injector. (DACInjector.dll) Exiting in 5 seconds...");
				Thread.Sleep(5000);
				return;
			}

			if ((result = Injection.Inject(dllPath, false)) == DACInjectionResult.RobloxNotFound)
			{

				Console.Write("Do you want to wait for Roblox to open before injection (y/n)? ");
				{
					string input = Console.ReadLine().ToLower();
					waitForRoblox = input[0] == 'y' && !input.Contains("no");

					if (input[0] == 'y' && input.Length > 2 && input[1] != 'e')
						Console.WriteLine("I'll just take that as a yes?");
				}
				Console.WriteLine();

				result = Injection.Inject(dllPath, waitForRoblox);
			}

			/* unload, and give result */
			Console.WriteLine("Injection result: {0}\r\nExiting in 5 seconds...", result.ToString());
			Injection.Unload();

			Thread.Sleep(5000);
		}
	}
}
