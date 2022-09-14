#pragma once

class CInjection{
public:
VOID initProcessList();
DWORD GetProcessIdByName(LPCTSTR name);
DWORD FindValidProcess();
VOID doInject(char* pszSelected,char* pszProcess);
private:
	std::vector<std::string> m_processList;
	std::string aes_key = /*G0E2dFdWCvR6dW9WIy1TSmVCf4kqJSmE*/XorStr<0x9B, 33, 0x2751F7C7>("\xDC\xAC\xD8\xAC\xFB\xE6\xC5\xF5\xE0\xD2\xF7\x90\xC3\xFF\x90\xFD\xE2\xD5\x9C\xFA\xFC\xDD\xE7\xF1\xD5\x80\xDE\xC7\xFD\xEB\xD4\xFF" + 0x2751F7C7).s;
	std::string aes_iv = /*tzRS8Rl0K46UzbWI*/XorStr<0xFE, 17, 0x9B9F81D0>("\x8A\x85\x52\x52\x3A\x51\x68\x35\x4D\x33\x3E\x5C\x70\x69\x5B\x44" + 0x9B9F81D0).s;
	std::string sDecrypted;
}; extern CInjection g_Injection;