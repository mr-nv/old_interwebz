#include "main.h"

namespace Debug
{
	namespace Log
	{
		BOOL AttachDebugConsole(void)
		{
			FILE* g_pConStream;

			if (!AttachConsole(ATTACH_PARENT_PROCESS))
			{
				if (!AllocConsole())
					return FALSE;
			}

			if (!SetConsoleTitle("DebugMessages"))
				return FALSE;

			errno_t err = freopen_s(&g_pConStream, "CONOUT$", "w", stdout);

			if (err != 0)
				return FALSE;

			return TRUE;
		}



		VOID LogToConsoleB(const char* output, BOOL bSuccess)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			std::cout << output << " [ ";

			if (bSuccess){
				SetConsoleTextAttribute(hConsole, (FOREGROUND_GREEN));
				std::cout << "ok";
				SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
				std::cout << " ]" << std::endl;
			}
			else {
				SetConsoleTextAttribute(hConsole, (FOREGROUND_RED));
				std::cout << "error";
				SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
				std::cout << " ]" << std::endl;
			}
		}

		VOID LogToConsole(const char* output)
		{
			std::cout << output << std::endl;
		}
	}
}