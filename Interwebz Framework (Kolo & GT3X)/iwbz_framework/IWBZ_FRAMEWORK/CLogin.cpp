#include "main.h"

CGlobals g_Globals;

std::string CLogin::getCurrentTime()
{
	//formt H-M:S
	time_t now = time(0);
	struct tm timestruct;
	char buf[256];
	timestruct = *localtime(&now);	
	strftime(buf, sizeof(buf),/*%M*/XorStr<0xBA, 3, 0xAB3FD5CA>("\x9F\xF6" + 0xAB3FD5CA).s, &timestruct);

	return buf;
}

void CLogin::GetMACAddress(std::string &sOut)
{
	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBufLen = sizeof(AdapterInfo);

	char *pszMacAddy = new char[256];

	GetAdaptersInfo(AdapterInfo, &dwBufLen);

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;

	do
	{
		sprintf(pszMacAddy, /*%02X:%02X:%02X:%02X:%02X:%02X*/XorStr<0xCD, 30, 0x4BB1266F>("\xE8\xFE\xFD\x88\xEB\xF7\xE3\xE6\x8D\xEC\xF2\xE8\xEB\x82\xE1\xF9\xED\xEC\x87\xDA\xC4\xD2\xD1\xBC\xDF\xC3\xD7\xDA\xB1" + 0x4BB1266F).s,
			pAdapterInfo->Address[0], pAdapterInfo->Address[1],
			pAdapterInfo->Address[2], pAdapterInfo->Address[3],
			pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

		sOut = pszMacAddy;
		delete[] pszMacAddy;
		return;

		pAdapterInfo = pAdapterInfo->Next;

	} while (pAdapterInfo);
}

/////////////////////////////////////////////
// Processes the login 
//! param - PCHAR pszUsername
//! param - PCHAR pszPassword
//! ret - VOID
// TODO: Whole function lel
/////////////////////////////////////////////
VOID CLogin::processLogin(char* pszUsername, char* pszPassword)
{
	std::string cryptedTime = Crypt::encrypt(getCurrentTime(), aes_key, aes_iv);	

	HW_PROFILE_INFO hwProfileInfo;

	//Hide api call
	GetCurrentHwProfileHidden(&hwProfileInfo);	
	
	std::string sMAC;
	GetMACAddress(sMAC);	

	char request[512];
	sprintf(request, /*_INTERWEBZ/login/login.php?u=%s&p=%s&hwid=%s*/XorStr<0xC6, 45, 0xEA8C5EB4>("\x99\x8E\x86\x9D\x8F\x99\x9B\x88\x8C\x95\xFF\xBD\xBD\xB4\xBD\xBB\xF9\xBB\xB7\xBE\xB3\xB5\xF2\xAD\xB6\xAF\xDF\x94\xDF\xC6\x97\xC3\x96\xDA\xCD\x9A\xCC\x83\x9B\x84\x8A\xD2\xD5\x82" + 0xEA8C5EB4).s, pszUsername, pszPassword, sMAC.c_str());

	g_Winsock.InitWinsock();
	g_Winsock.Connect(/*37.187.236.54*/XorStr<0xDB, 14, 0x41843CD4>("\xE8\xEB\xF3\xEF\xE7\xD7\xCF\xD0\xD0\xD2\xCB\xD3\xD3" + 0x41843CD4).s);
	CString cstResponse;
	g_Winsock.SendHttpGet(request, cstResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();
	
	
	std::string sRet((LPCTSTR)cstResponse);	

	std::cout << sRet << std::endl;

	if (strcmp(sRet.c_str(),/*error*/XorStr<0x34, 6, 0x4DA3BC70>("\x51\x47\x44\x58\x4A" + 0x4DA3BC70).s) == 0)
	{
		m_login.bSuccess = FALSE;
		return;
	}


	std::string sDecrypted = Crypt::decrypt(sRet, aes_key, aes_iv);	

	if (strstr(sDecrypted.c_str(),/*hwiderror*/XorStr<0x79, 10, 0x2F105E98>("\x11\x0D\x12\x18\x18\x0C\x0D\xEF\xF3" + 0x2F105E98).s))
	{
		MessageBoxHidden(0,/*hwiderror*/XorStr<0x79, 10, 0x2F105E98>("\x11\x0D\x12\x18\x18\x0C\x0D\xEF\xF3" + 0x2F105E98).s,/*Error*/XorStr<0x66, 6, 0x9F5A486C>("\x23\x15\x1A\x06\x18" + 0x9F5A486C).s, 0);
		ExitProcessHidden(0);
		int* p = 0;
		*p = 0;
	}

	std::string sVeri = sDecrypted.substr(0, 2);

	if (strcmp(sVeri.c_str(), getCurrentTime().c_str()) == 0)
		m_login.bSuccess = TRUE;	

	std::string sGames = sDecrypted.substr(2);

	if (strcmp(sGames.c_str(),/*all*/XorStr<0x80, 4, 0xC3A553DE>("\xE1\xED\xEE" + 0xC3A553DE).s) == 0)
	{
		m_login.bSuccess = TRUE;
		m_login.bCSGO = TRUE;
		m_login.bL4D2 = TRUE;
		//m_login.bCSS = TRUE;
		//m_login.bTF2 = TRUE;
	}

	//TODO: Crawl games string for other games, will be added if we offer more games LoL
	
	if (strstr(sGames.c_str(),/*csgo*/XorStr<0xFF, 5, 0xEDB3B1D1>("\x9C\x73\x66\x6D" + 0xEDB3B1D1).s))
	{
		m_login.bSuccess = TRUE;
		m_login.bCSGO = TRUE;
	}

	if (strstr(sGames.c_str(),/*L4D2*/XorStr<0x24, 5, 0x59704319>("\x68\x11\x62\x15" + 0x59704319).s))
	{
		m_login.bSuccess = TRUE;
		m_login.bL4D2 = TRUE;
	}


	if (strstr(sGames.c_str(),/*css*/XorStr<0x29, 4, 0x54CEDF33>("\x4A\x59\x58" + 0x54CEDF33).s))
		//m_login.bCSS = TRUE;

	if (strstr(sGames.c_str(),/*tf2*/XorStr<0x86, 4, 0x53B95F72>("\xF2\xE1\xBA" + 0x53B95F72).s))
		m_login.bTF2 = TRUE;

	
}

/////////////////////////////////////////////
//Returns the users current login state
//! param - 
//! ret - BOOL
// TODO: Add code obfuscation
/////////////////////////////////////////////
login CLogin::getSession()
{
	return m_login;
}


BOOL CLogin::IsOutdated()
{
	std::string sMAC;
	GetMACAddress(sMAC);
		
	g_Winsock.InitWinsock();
	g_Winsock.Connect(/*37.187.236.54*/XorStr<0xDB, 14, 0x41843CD4>("\xE8\xEB\xF3\xEF\xE7\xD7\xCF\xD0\xD0\xD2\xCB\xD3\xD3" + 0x41843CD4).s);
	CString cstResponse;
	char request[512];
	sprintf_s(request, /*_INTERWEBZ/login/version.php?hwid=%s*/XorStr<0x76, 37, 0xBF82BAA6>("\x29\x3E\x36\x2D\x3F\x29\x2B\x38\x3C\x25\xAF\xED\xED\xE4\xED\xEB\xA9\xF1\xED\xFB\xF9\xE2\xE3\xE3\xA0\xFF\xF8\xE1\xAD\xFB\xE3\xFC\xF2\xAA\xBD\xEA" + 0xBF82BAA6).s, sMAC.c_str());
	g_Winsock.SendHttpGet(request, cstResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();

	int iServerVersion = atoi(cstResponse);		

#ifdef DEBUG
	char pszDebug[1024];
	sprintf(pszDebug,/*Server version: %i*/XorStr<0x53, 19, 0xBF5EEB0E>("\x00\x31\x27\x20\x32\x2A\x79\x2C\x3E\x2E\x2E\x37\x30\x0E\x5B\x42\x46\x0D" + 0xBF5EEB0E).s, iServerVersion);
	Debug::Log::LogToConsole(pszDebug);
#endif

	if (!IsWindows7OrGreater())
	{
		MessageBoxHidden(0,/*Unsupported OS*/XorStr<0x99, 15, 0xD89956AE>("\xCC\xF4\xE8\xE9\xED\xEE\xF0\xD2\xD5\xC7\xC7\x84\xEA\xF5" + 0xD89956AE).s,/*Error*/XorStr<0x02, 6, 0x9CBE23DA>("\x47\x71\x76\x6A\x74" + 0x9CBE23DA).s, 0);
		ExitProcessHidden(0);
		int* p = 0;
		*p = 0;
	}

	//blacklisted
	if (iServerVersion > 999)
	{
		MessageBoxHidden(0, /*Startup error! Please open a support thread*/XorStr<0xCB, 44, 0x10D96B9C>("\x98\xB8\xAC\xBC\xBB\xA5\xA1\xF2\xB6\xA6\xA7\xB9\xA5\xF9\xF9\x8A\xB7\xB9\xBC\xAD\xBA\xC0\x8E\x92\x86\x8A\xC5\x87\xC7\x9B\x9C\x9A\x9B\x83\x9F\x9A\xCF\x84\x99\x80\x96\x95\x91" + 0x10D96B9C).s, /*Error*/XorStr<0x9C, 6, 0x9140948F>("\xD9\xEF\xEC\xF0\xD2" + 0x9140948F).s, 0);
		ExitProcessHidden(0);
		int* p = 0;
		*p = 0;				
	}

	if (14 < iServerVersion)
		return TRUE;

	return FALSE;
}

BOOL CLogin::isValidInput(char* pszInput)
{
	//Empty?
	if (strlen(pszInput) == 0)
		return FALSE;

	//Got space?
	for (unsigned int ax = 0; ax < strlen(pszInput); ax++)
	{
		if (pszInput[ax] == char(32))
			return FALSE;
	}

	return TRUE;
}

VOID addComboEntry(HWND hCombo, LPCTSTR entry)
{
	SendMessageHidden(hCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCTSTR)entry));
}

VOID CLogin::initComboBox(HWND hCombo)
{

	if (m_login.bCSGO == TRUE)
		addComboEntry(hCombo, /*CS:GO*/XorStr<0xC0, 6, 0x2F0C09F0>("\x83\x92\xF8\x84\x8B" + 0x2F0C09F0).s);
	if (m_login.bCSS == TRUE)
		addComboEntry(hCombo, /*CS:S*/XorStr<0x8D, 5, 0x1A5C845A>("\xCE\xDD\xB5\xC3" + 0x1A5C845A).s);
	if (m_login.bTF2 == TRUE)
		addComboEntry(hCombo,/*TF2*/XorStr<0xB0, 4, 0xC2496075>("\xE4\xF7\x80" + 0xC2496075).s);
	if (m_login.bL4D2 == TRUE)
		addComboEntry(hCombo,/*L4D2*/XorStr<0x24, 5, 0x59704319>("\x68\x11\x62\x15" + 0x59704319).s);
}