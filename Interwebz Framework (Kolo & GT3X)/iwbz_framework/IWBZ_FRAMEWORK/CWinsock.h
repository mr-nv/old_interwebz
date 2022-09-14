#pragma once

class CWinsock
{
public:
	CWinsock(VOID);
	~CWinsock(VOID);

	VOID InitWinsock(VOID);
	VOID Connect(const char* pszServerUrl);
	VOID SendHttpGet(const char* pszFileUrl, CString &sDestBuffer);	
	VOID Disconnect(VOID);
	VOID CleanUp(VOID);
private:
	SOCKET m_ConnectedSocket;
	const char* m_pszConnectedUrl;

}; extern CWinsock g_Winsock;