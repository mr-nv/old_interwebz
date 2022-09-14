#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0600 
#define DEBUG
#define CHECKUP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <CommCtrl.h>
#include <winnt.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <iostream>
#include <atlstr.h>
#include <string>
#include <mmsystem.h>
#include <WinInet.h>
#include <time.h>
#include <vector>
#include <debugapi.h>
#include <stdlib.h>  
#include <gdiplus.h>
#include < Winternl.h>
#include <Iphlpapi.h>
#include <VersionHelpers.h>
#include <ShlObj.h>
#include <fstream>
#include <DbgHelp.h>
#include <Psapi.h>
#include <gdiplus.h>
#include <utility>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment (lib, "ws2_32.lib" )
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib, "wininet")
#pragma comment(lib,"cryptlib.lib")
#pragma comment( lib, "Msimg32" )
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Dbghelp.lib")
#pragma comment (lib, "urlmon.lib")

//crypto
#include "cryptopp562/base64.h"
#include "cryptopp562/cryptlib.h"
#include "cryptopp562/hex.h"
#include "cryptopp562/filters.h"
#include "cryptopp562/aes.h"
#include "cryptopp562/modes.h"

using CryptoPP::Exception;
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::AES;
using CryptoPP::CFB_Mode;
using CryptoPP::CBC_Mode;
using CryptoPP::Base64Encoder;
using CryptoPP::Base64Decoder;

#include "WinAPI.h"
#include "xor.h"
#include "CWinsock.h"
#include "CLogin.h"
#include "AES.h"
#include "CGlobals.h"
#include "Injection.h"
#include "LoadLibraryR.h"
#include "Security.h"
#include "Debug.h"
#include "base64.h"