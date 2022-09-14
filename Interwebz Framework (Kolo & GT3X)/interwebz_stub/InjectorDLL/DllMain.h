#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <psapi.h>
#include <atlstr.h>
#include <time.h>
#include <process.h>
#include <vector>
#include <Iphlpapi.h>
#include <map>
#include <ostream>
#include <gdiplus.h>
#include <utility>
#include <WinInet.h>
#include <ShlObj.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib, "ws2_32.lib" )
#pragma comment(lib,"cryptlib.lib")
#pragma comment(lib, "wininet")
#pragma comment(lib, "IPHLPAPI.lib")
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

#include "xor.h"
#include "WinAPI.h"
#include "CWinsock.h"
#include "AES.h"
#include "CSecurity.h"
#include "base64.h"
#include "LoadLibraryR.h"
