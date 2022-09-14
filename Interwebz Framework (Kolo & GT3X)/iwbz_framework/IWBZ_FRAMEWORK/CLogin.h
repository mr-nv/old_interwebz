#pragma once

struct login
{
	BOOL bSuccess;
	BOOL bCSGO;
	BOOL bCSS;
	BOOL bTF2;
	BOOL bL4D2;
};

class CLogin
{
public:
	void GetMACAddress(std::string &sOut);
	BOOL isValidInput(char* pszInput);
	std::string getCurrentTime();
	VOID processLogin(char* pszUsername, char* pszPassword);
	VOID initComboBox(HWND hCombo);
	login getSession();
	BOOL IsOutdated();		
private:
	login m_login;
	std::string aes_key = /*KEh1Iwu1PPBiegUo25OqzY2z9su5I6IK*/XorStr<0x53, 33, 0xA96EB504>("\x18\x11\x3D\x67\x1E\x2F\x2C\x6B\x0B\x0C\x1F\x37\x3A\x07\x34\x0D\x51\x51\x2A\x17\x1D\x31\x5B\x10\x52\x1F\x18\x5B\x26\x46\x38\x39" + 0xA96EB504).s;
	std::string aes_iv = /*S1qX6c2ScZqEDyFC*/XorStr<0x96, 17, 0xA8ED57A1>("\xC5\xA6\xE9\xC1\xAC\xF8\xAE\xCE\xFD\xC5\xD1\xE4\xE6\xDA\xE2\xE6" + 0xA8ED57A1).s;

}; extern CLogin g_Login;