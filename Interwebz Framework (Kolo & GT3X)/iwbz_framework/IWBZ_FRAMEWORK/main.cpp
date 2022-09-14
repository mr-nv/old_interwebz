#include "main.h"
#include "Injection.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////////
/*
INTERWEBZ FRAMEWORK 1.0
*/
//////////////////////////////////////////////////////////////////////////

CLogin g_Login;
CInjection g_Injection;


/////////////////////////////////////////////////////////////////////////
//Loginwindow variables
#define ID_BUTTON_LOGIN 1
HWND hLoginButton, hUserLabel, hPassLabel, hUserEdit, hPassEdit,hVersion;
HWND hwndMain, hwndLogin;
HDC hdc;
COLORREF crBk = RGB(255, 64, 64);
char pszUsername[256];
char pszPassword[256];
//////////////////////////////////////////////////////////////////////////
//Loginwindow
LRESULT CALLBACK LoginProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	

	std::string sMain = /*Main*/XorStr<0x07, 5, 0x3B0E552A>("\x4A\x69\x60\x64" + 0x3B0E552A).s;
	char pszBuild[1024];
	switch (message)
	{
	case WM_CREATE:
		hUserLabel = CreateWindowExAHidden(NULL, /*static*/XorStr<0xFD, 7, 0x4589EA5B>("\x8E\x8A\x9E\x74\x68\x61" + 0x4589EA5B).s, /*Username*/XorStr<0xB6, 9, 0x7D2AF6F4>("\xE3\xC4\xDD\xCB\xD4\xDA\xD1\xD8" + 0x7D2AF6F4).s,
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			60, 45, 200, 20,
			hwnd, (HMENU)1, GetModuleHandleHidden(NULL), NULL);
		SendMessageHidden(hUserLabel, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);

		hPassLabel = CreateWindowExAHidden(NULL, /*static*/XorStr<0xFD, 7, 0x4589EA5B>("\x8E\x8A\x9E\x74\x68\x61" + 0x4589EA5B).s, /*Password*/XorStr<0x63, 9, 0x0FC1B93E>("\x33\x05\x16\x15\x10\x07\x1B\x0E" + 0x0FC1B93E).s,
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			60, 80, 200, 20,
			hwnd, (HMENU)1, GetModuleHandleHidden(NULL), NULL);
		SendMessageHidden(hPassLabel, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);

		hUserEdit = CreateWindowExAHidden(WS_EX_CLIENTEDGE, /*edit*/XorStr<0x98, 5, 0x411DC342>("\xFD\xFD\xF3\xEF" + 0x411DC342).s, "",
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			60, 60, 200, 20,
			hwnd, (HMENU)1, GetModuleHandleHidden(NULL), NULL);
		SendMessageHidden(hUserEdit, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);

		hPassEdit = CreateWindowExAHidden(WS_EX_CLIENTEDGE, /*edit*/XorStr<0x98, 5, 0x411DC342>("\xFD\xFD\xF3\xEF" + 0x411DC342).s, "",
			WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_PASSWORD,
			60, 95, 200, 20,
			hwnd, (HMENU)1, GetModuleHandleHidden(NULL), NULL);
		SendMessageHidden(hPassEdit, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);
		
		hLoginButton = CreateWindowExAHidden(0, /*button*/XorStr<0x7D, 7, 0x0E4C1315>("\x1F\x0B\x0B\xF4\xEE\xEC" + 0x0E4C1315).s, /*Login*/XorStr<0x78, 6, 0x26369EE2>("\x34\x16\x1D\x12\x12" + 0x26369EE2).s,
			WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON ,
			60, 130, 200, 20,
			hwnd, (HMENU)ID_BUTTON_LOGIN, GetModuleHandleHidden(NULL), NULL);
		SendMessageHidden(hLoginButton, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);

		sprintf(pszBuild, /*Build: %iB*/XorStr<0xC2, 11, 0x8D07BEF5>("\x80\xB6\xAD\xA9\xA2\xFD\xE8\xEC\xA3\x89" + 0x8D07BEF5).s, 1);

		hVersion = CreateWindowExAHidden(0, /*static*/XorStr<0xFD, 7, 0x4589EA5B>("\x8E\x8A\x9E\x74\x68\x61" + 0x4589EA5B).s, pszBuild,
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			280, 140, 200, 20,
			hwnd, NULL, GetModuleHandleHidden(NULL), NULL);
		SendMessageHidden(hVersion, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);
		
		hwndMain = CreateWindowExAHidden(0, sMain.c_str(), /*INTERWEBZ*/XorStr<0xA7, 10, 0xA2B0692F>("\xEE\xE6\xFD\xEF\xF9\xFB\xE8\xEC\xF5" + 0xA2B0692F).s, WS_SYSMENU, 800, 350, 350, 200, NULL, NULL, GetModuleHandleHidden(NULL), NULL);

	case WM_CTLCOLOREDIT:	 	
		hdc = (HDC)wParam;
		SetBkColorHidden(hdc, crBk); // Set to red
		SetTextColorHidden(hdc, RGB(255, 255, 255));
		SetDCBrushColorHidden(hdc, crBk);
		return (LRESULT)GetStockObjectHidden(DC_BRUSH);
	case WM_CTLCOLORSTATIC:
		hdc = (HDC)wParam;
		SetTextColorHidden(hdc, RGB(133, 133, 133));
		SetBkModeHidden(hdc, TRANSPARENT);
		return (LRESULT)GetStockObjectHidden(NULL_BRUSH);
	case WM_COMMAND:
		
		switch (wParam)
		{
		case ID_BUTTON_LOGIN:
			GetWindowTextHidden(hUserEdit, pszUsername, 256);
			GetWindowTextHidden(hPassEdit, pszPassword, 256);			
						
			if (!g_Login.isValidInput(pszUsername))
			{
				MessageBoxHidden(0,/*Invalid username*/XorStr<0x29, 17, 0x64209286>("\x60\x44\x5D\x4D\x41\x47\x4B\x10\x44\x41\x56\x46\x5B\x57\x5A\x5D" + 0x64209286).s, /*Error*/XorStr<0xB3, 6, 0x146401F1>("\xF6\xC6\xC7\xD9\xC5" + 0x146401F1).s,0);
				break;
			}
			if (!g_Login.isValidInput(pszPassword))
			{
				MessageBoxHidden(0,/*Invalid password*/XorStr<0x06, 17, 0x843B3676>("\x4F\x69\x7E\x68\x66\x62\x68\x2D\x7E\x6E\x63\x62\x65\x7C\x66\x71" + 0x843B3676).s,/*Error*/XorStr<0xB3, 6, 0x146401F1>("\xF6\xC6\xC7\xD9\xC5" + 0x146401F1).s, 0);
				break;
			}
						
			//Process login and acquire session
			g_Login.processLogin(pszUsername,pszPassword);			

			if (g_Login.getSession().bSuccess == TRUE)
			{						
				//Show injection window
				ShowWindowHidden(hwndMain,SW_SHOW);
				ShowWindowHidden(hwndLogin, SW_HIDE);
			}
			else			
				MessageBoxHidden(0, /*Login failed*/XorStr<0x84, 13, 0x2905274D>("\xC8\xEA\xE1\xEE\xE6\xA9\xEC\xEA\xE5\xE1\xEB\xEB" + 0x2905274D).s, /*Error*/XorStr<0xB3, 6, 0x146401F1>("\xF6\xC6\xC7\xD9\xC5" + 0x146401F1).s, 0);			
		}
		break;
	case WM_DESTROY:
		PostQuitMessageHidden(0);
		break;
	}	
	return DefWindowProcHidden(hwnd, message, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////////
//MainWindow variables
#define ID_BUTTON_INJECT 2
HWND hCombo, hComboProcess, hProgressLabel, hInjectButton, hHackLabel, hProcessLabel;
BOOL Isx64Process(DWORD pid)
{
	HANDLE process = OpenProcessHidden(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);	
	PROCESS_BASIC_INFORMATION pbi;
	RtlZeroMemory(&pbi, sizeof(PROCESS_BASIC_INFORMATION));
	NtQueryInformationProcessHidden(process, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);

	CloseHandleHidden(process);

	if (pbi.PebBaseAddress) { return FALSE; }

	return TRUE;
}

VOID getValidProcesses()
{
	PROCESSENTRY32 pe32;
	HANDLE snapshot = NULL;	

	snapshot = CreateToolhelp32SnapshotHidden(TH32CS_SNAPPROCESS, 0);
	if (snapshot != INVALID_HANDLE_VALUE) {
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32FirstHidden(snapshot, &pe32)) {
			do {
				if (strstr(pe32.szExeFile, "System") || strstr(pe32.szExeFile,"steam") || strstr(pe32.szExeFile,"svchost.exe") || strstr(pe32.szExeFile,"notepad.exe"))
					continue;

				if(!Isx64Process(pe32.th32ProcessID))
					SendMessageHidden(hComboProcess, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCTSTR)pe32.szExeFile));								

			} while (Process32NextHidden(snapshot, &pe32));
		}
		CloseHandleHidden(snapshot);
	}	
}
/////////////////////////////////////////////////////////////////////////
//MainWindow 
LRESULT CALLBACK MainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	char pszSelected[256];	
	char pszSelectedProcess[256];
	switch (message)
	{
	case WM_CREATE:

		hHackLabel = CreateWindowExAHidden(0, /*static*/XorStr<0xFD, 7, 0x4589EA5B>("\x8E\x8A\x9E\x74\x68\x61" + 0x4589EA5B).s, /*Select a hack*/XorStr<0x7D, 14, 0xA5E99BAB>("\x2E\x1B\x13\xE5\xE2\xF6\xA3\xE5\xA5\xEE\xE6\xEB\xE2" + 0xA5E99BAB).s,
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			60, 4, 200, 20,
			hwnd, NULL, GetModuleHandleHidden(NULL), NULL);
		SendMessageHidden(hHackLabel, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);

		hCombo = CreateWindowExAHidden(0, /*COMBOBOX*/XorStr<0xDF, 9, 0x807E712F>("\x9C\xAF\xAC\xA0\xAC\xA6\xAA\xBE" + 0x807E712F).s, NULL,
			WS_CHILD | WS_TABSTOP | WS_VISIBLE | CBS_DROPDOWN,
			60, 20, 200, 100,
			hwnd, NULL, GetModuleHandleHidden(NULL), NULL);
		SendMessageHidden(hCombo, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);
		
		hProcessLabel = CreateWindowExAHidden(0, /*static*/XorStr<0xFD, 7, 0x4589EA5B>("\x8E\x8A\x9E\x74\x68\x61" + 0x4589EA5B).s, /*Select a process*/XorStr<0x76, 17, 0x466D0886>("\x25\x12\x14\x1C\x19\x0F\x5C\x1C\x5E\x0F\xF2\xEE\xE1\xE6\xF7\xF6" + 0x466D0886).s,
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			60, 50, 200, 20,
			hwnd, NULL, GetModuleHandleHidden(NULL), NULL);
		SendMessageHidden(hProcessLabel, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);

		hComboProcess = CreateWindowExAHidden(0, /*COMBOBOX*/XorStr<0xDF, 9, 0x807E712F>("\x9C\xAF\xAC\xA0\xAC\xA6\xAA\xBE" + 0x807E712F).s, NULL,
			WS_CHILD | WS_TABSTOP | WS_VSCROLL | WS_VISIBLE | CBS_DROPDOWN,
			60, 66, 200, 100,
			hwnd, NULL, GetModuleHandleHidden(NULL), NULL);
		SendMessageHidden(hComboProcess, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);

		getValidProcesses();

		g_Globals.hProgress = CreateWindowExAHidden(0, PROGRESS_CLASS, NULL,
			WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
			60, 100, 200, 17,
			hwnd, NULL, GetModuleHandleHidden(NULL), NULL);
		SendMessage(g_Globals.hProgress, PBM_SETSTEP, (WPARAM)20, 0);

		hInjectButton = CreateWindowExAHidden(0, /*button*/XorStr<0x7D, 7, 0x0E4C1315>("\x1F\x0B\x0B\xF4\xEE\xEC" + 0x0E4C1315).s, /*Inject*/XorStr<0xD7, 7, 0xFBC752B1>("\x9E\xB6\xB3\xBF\xB8\xA8" + 0xFBC752B1).s,
				WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON,
				60, 120, 200, 20,
				hwnd, (HMENU)ID_BUTTON_INJECT, GetModuleHandleHidden(NULL), NULL);
			SendMessageHidden(hInjectButton, WM_SETFONT, (WPARAM)g_Globals.hFont, 0);					
		break;
	case WM_CTLCOLORSTATIC:
		hdc = (HDC)wParam;
		SetTextColorHidden(hdc, RGB(133, 133, 133));
		SetBkModeHidden(hdc, TRANSPARENT);
		return (LRESULT)GetStockObjectHidden(NULL_BRUSH);
	case WM_SHOWWINDOW:
		g_Login.initComboBox(hCombo);
		break;

	case WM_COMMAND:

		switch (wParam)
		{
		case ID_BUTTON_INJECT:
			GetWindowTextHidden(hCombo, pszSelected, 256);		
			GetWindowTextHidden(hComboProcess, pszSelectedProcess, 256);

			if (!g_Login.isValidInput(pszSelected))
			{
				MessageBoxHidden(0,/*No hack selected*/XorStr<0x77, 17, 0xECDE6D72>("\x39\x17\x59\x12\x1A\x1F\x16\x5E\x0C\xE5\xED\xE7\xE0\xF0\xE0\xE2" + 0xECDE6D72).s,/*Error*/XorStr<0x8B, 6, 0xA3A7C43E>("\xCE\xFE\xFF\xE1\xFD" + 0xA3A7C43E).s, 0);
				break;
			}
			SendMessageHidden(g_Globals.hProgress, PBM_STEPIT, 0, 0);

			g_Injection.doInject(pszSelected,pszSelectedProcess);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessageHidden(0);
		break;
	}	
	return DefWindowProcHidden(hwnd, message, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hI, HINSTANCE hPrI, PSTR szCmdLine, int iCmdShow)
{	
	//Debug::Log::AttachDebugConsole();

	initSecurity();
					
	__asm
	{
		mov eax, fs:[0x30] // PEB
		mov eax, [eax + 0x0c] // PEB_LDR_DATA
		mov eax, [eax + 0x0c] // InOrderModuleList
		mov dword ptr[eax + 0x20], 0x100000000 // SizeOfImage
	}
	
	
	//Is outdated?
	if (g_Login.IsOutdated())
	{
		MessageBoxHidden(0,/*Client version doesn't match server version*/XorStr<0x6D, 44, 0xF58DE7CA>("\x2E\x02\x06\x15\x1F\x06\x53\x02\x10\x04\x04\x11\x16\x14\x5B\x18\x12\x1B\x0C\xEE\xA6\xF6\xA3\xE9\xE4\xF2\xE4\xE0\xA9\xF9\xEE\xFE\xFB\xEB\xFD\xB0\xE7\xF7\xE1\xE7\xFC\xF9\xF9" + 0xF58DE7CA).s,/*Error*/XorStr<0xC5, 6, 0xBC358DF8>("\x80\xB4\xB5\xA7\xBB" + 0xBC358DF8).s, 0);
		ExitProcessHidden(0);
	}
#ifndef CHECKUP

	//Is Steam running?
	if (g_Injection.GetProcessIdByName(/*steam.exe*/XorStr<0x9A, 10, 0x0BA4BA1F>("\xE9\xEF\xF9\xFC\xF3\xB1\xC5\xD9\xC7" + 0x0BA4BA1F).s) != 0x0)
	{
		if (MessageBox(0,/*For safety reasons close steam before injecting and restart the client!*/XorStr<0xD6, 72, 0xFF52F931>("\x90\xB8\xAA\xF9\xA9\xBA\xBA\xB8\xAA\xA6\xC0\x93\x87\x82\x97\x8A\x88\x94\xC8\x8A\x86\x84\x9F\x88\xCE\x9C\x84\x94\x93\x9E\xD4\x97\x93\x91\x97\x8B\x9F\xDB\x95\x93\x94\x9A\x63\x75\x6B\x6D\x63\x25\x67\x69\x6C\x29\x78\x6E\x7F\x79\x6F\x7D\x64\x31\x66\x7B\x71\x35\x75\x7B\x71\x7C\x74\x6F\x3D" + 0xFF52F931).s, /*Steam*/XorStr<0x6A, 6, 0xBA1297E6>("\x39\x1F\x09\x0C\x03" + 0xBA1297E6).s, MB_OK) == IDOK)
			ExitProcessHidden(0);
	}


	TCHAR ExeName[MAX_PATH];
	GetModuleFileNameHidden(NULL, ExeName, MAX_PATH);
	TCHAR DriveLetter = ExeName[0];

	if(DriveLetter == 'C' || DriveLetter == 'c')
	{
		MessageBoxHidden(0,/*Please run interwebz from a thumbdrive*/XorStr<0x04,39,0xDC1662B3>("\x54\x69\x63\x66\x7B\x6C\x2A\x79\x79\x63\x2E\x66\x7E\x65\x77\x61\x63\x70\x74\x6D\x38\x7F\x68\x74\x71\x3D\x7F\x3F\x54\x49\x57\x4E\x46\x41\x54\x4E\x5E\x4C"+0xDC1662B3).s,/*Error*/XorStr<0x3F,6,0x0CCACD9B>("\x7A\x32\x33\x2D\x31"+0x0CCACD9B).s,0);
		ExitProcessHidden(0);
	}




	//Filename check
	TCHAR chPath[MAX_PATH];
	GetModuleFileNameHidden(NULL, chPath, MAX_PATH);		

	if (!strstr(chPath, /*notepad.exe*/XorStr<0x43, 12, 0xB95C9272>("\x2D\x2B\x31\x23\x37\x29\x2D\x64\x2E\x34\x28" + 0xB95C9272).s))
		if (MessageBoxHidden(0,/*For security reasons we recommend to not change the file name!*/XorStr<0x6F, 63, 0x3B70575E>("\x29\x1F\x03\x52\x00\x11\x16\x03\x05\x11\x0D\x03\x5B\x0E\x18\x1F\x0C\xEF\xEF\xF1\xA3\xF3\xE0\xA6\xF5\xED\xEA\xE5\xE6\xE1\xE8\xE0\xEB\xB0\xE5\xFD\xB3\xFA\xFA\xE2\xB7\xFB\xF1\xFB\xF5\xFB\xF8\xBE\xEB\xC8\xC4\x82\xC5\xCD\xC9\xC3\x87\xC6\xC8\xC7\xCE\x8D" + 0x3B70575E).s,/*Error*/XorStr<0x36, 6, 0xAF29B529>("\x73\x45\x4A\x56\x48" + 0xAF29B529).s, MB_OK) == IDOK)
			ExitProcessHidden(0);
	
#endif
	

	g_Globals.initFont();
	InitCommonControlsHidden();		
	
	std::string sLogin = /*Login*/XorStr<0x97, 6, 0xC2191AB7>("\xDB\xF7\xFE\xF3\xF5" + 0xC2191AB7).s;
	std::string sMain = /*Main*/XorStr<0x07, 5, 0x3B0E552A>("\x4A\x69\x60\x64" + 0x3B0E552A).s;
	//HICON hIcon = LoadIconHidden(hI, MAKEINTRESOURCE(IDI_ICON1));

	HBRUSH brush = CreateSolidBrushHidden(RGB(32, 32, 32));
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = LoginProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hI;
	wc.hIcon = 0;
	wc.hCursor = LoadCursorHidden(NULL, IDC_ARROW);	
	wc.hbrBackground = CreatePatternBrushHidden(LoadBitmapHidden(hI, MAKEINTRESOURCE(IDB_BITMAP2))); 
	wc.lpszMenuName = NULL;
	wc.lpszClassName = sLogin.c_str();
		
	RegisterClassHidden(&wc);

	//wc.hIcon = LoadIconHidden(hI, MAKEINTRESOURCE(IDI_ICON1));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = sMain.c_str();
	wc.lpfnWndProc = MainProc;
	wc.hbrBackground = brush;

	RegisterClassHidden(&wc);
	
	hwndLogin = CreateWindowExAHidden(0, sLogin.c_str(), /*INTERWEBZ*/XorStr<0xA7, 10, 0xA2B0692F>("\xEE\xE6\xFD\xEF\xF9\xFB\xE8\xEC\xF5" + 0xA2B0692F).s, WS_SYSMENU, 750, 400, 350, 200, NULL, NULL, hI, NULL);

	ShowWindowHidden(hwndLogin, iCmdShow);
	UpdateWindowHidden(hwndLogin);
		
	
	//g_Security.GetConnectionLog();
	
		
	MSG msg;
	
	while (GetMessageHidden(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessageHidden(hwndLogin, &msg))
		{
			TranslateMessageHidden(&msg);
			DispatchMessageHidden(&msg);
		}
	}	
	return msg.wParam;	
}
