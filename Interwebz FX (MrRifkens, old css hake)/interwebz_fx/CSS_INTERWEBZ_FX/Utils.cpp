#include "Includes.h"

//typedef void( __cdecl* _PVFI )( int );
//extern "C" const _PVFI _FPinit;
//extern "C" void __cdecl _initp_misc_cfltcvt_tab( void );
std::map< DWORD, std::string > alreadyConverted;
namespace Base
{
	namespace Utils {

		ULONG PatternSearch ( std::string ModuleName, PBYTE Pattern, std::string Mask,
			ULONG uCodeBase = 0x0, ULONG uSizeOfCode = 0x0 )
		{
			BOOL bPatternDidMatch = FALSE;
			HMODULE hModule = GetModuleHandle( ModuleName.c_str() );

			if ( !hModule )
				return 0x0;

			PIMAGE_DOS_HEADER pDosHeader = PIMAGE_DOS_HEADER( hModule );
			if ( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
				return 0x0;

			PIMAGE_NT_HEADERS peHeader = PIMAGE_NT_HEADERS( ( ULONG )( hModule ) + pDosHeader->e_lfanew );
			if ( peHeader->Signature != IMAGE_NT_SIGNATURE )
				return 0x0;

			PIMAGE_OPTIONAL_HEADER pOptionalHeader = &peHeader->OptionalHeader;

			//if ( uCodeBase == 0x0 )
				uCodeBase = ( ULONG )( hModule ) + pOptionalHeader->BaseOfCode;

			//if ( uSizeOfCode == 0x0 )
				uSizeOfCode = pOptionalHeader->SizeOfCode;

			ULONG uArraySize = Mask.length();

			if ( !uCodeBase || !uSizeOfCode || !uArraySize )
				return 0x0;

			for ( size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++ )
			{
				for ( size_t t = 0; t < uArraySize; t++ )
				{
					if ( *( ( PBYTE ) i + t ) == Pattern[t] || Mask.c_str() [t] == '?' )
						bPatternDidMatch = TRUE;

					else
					{
						bPatternDidMatch = FALSE;
						break;
					}
				}

				if ( bPatternDidMatch )
					return i;
			}

			return 0x0;
		}

		void CRT_initFP ( void )
		{
			//if ( _FPinit != NULL )
			//	( *_FPinit )( TRUE );
			//_initp_misc_cfltcvt_tab();
		}

		bool AttachDebugConsole( void )
		{
			FILE* g_pConStream;

			if ( !AttachConsole( ATTACH_PARENT_PROCESS ) )
			{
				if ( !AllocConsole() )
					return false;
			}

			if ( !SetConsoleTitle( "DebugMessages" ) )
				return false;

			errno_t err = freopen_s( &g_pConStream, "CONOUT$", "w", stdout );

			if ( err != 0 )
				return false;

			return true;
		}

		void sleep ( long ms )
		{
			clock_t end, current = clock();
			for ( end = current + ms; current < end; current = clock() );
		}

		void ShowConsoleCursor( BOOL bShow )
		{
			static HANDLE hOut;
			static BOOL firstTime = TRUE;
			CONSOLE_CURSOR_INFO cursorInfo;

			if ( firstTime )
			{
				hOut = GetStdHandle( STD_OUTPUT_HANDLE );
				firstTime = FALSE;
			}

			cursorInfo.dwSize = 10;
			cursorInfo.bVisible = bShow;
			SetConsoleCursorInfo( hOut, &cursorInfo );
		}

		void dup ( unsigned char character, int n )
		{
			for ( int i = 0; i < n; i++ )
				std::cout << character;
		}

		void SetColor ( unsigned int col )
		{
			HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
			SetConsoleTextAttribute( hConsole, col );
		}

		void SetPos ( int x, int y )
		{
			COORD coord = { x, y };
			SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
		}

		void printfAligned( std::string text, int padWhitespace )
		{
			int strLength = text.length();
			padWhitespace -= strLength;
			for ( unsigned int i = 0; i < padWhitespace; i++ )
				std::cout << " ";
		}

		void printfCenter( const char* s )
		{
			int l = strlen( s );
			int pos = ( int )( ( 74 - l ) / 2 );
			for ( int i = 0; i < pos; i++ )
				std::cout << " ";

			std::cout << s;
		}

		const std::string& XORDecrypt(const char *cryptedString, DWORD dwKey1, DWORD dwKey2)
		{
			if( alreadyConverted.find( (DWORD)cryptedString ) != alreadyConverted.end() ) {
				return alreadyConverted[ (DWORD)cryptedString ];
			}
			std::string& str = alreadyConverted[ (DWORD)cryptedString ];
			int l = strlen( cryptedString );
			for( int i = 0; i < l; i++ ) {
				char p = cryptedString[ i ];
				int n = i % 8;
				char c = 0;
				switch( n ) {
				case 0: c = (char)( ( dwKey1 >> 24 ) & 0xFF ); break;
				case 1: c = (char)( ( dwKey1 >> 16 ) & 0xFF ); break;
				case 2: c = (char)( ( dwKey1 >> 8 ) & 0xFF ); break;
				case 3: c = (char)( ( dwKey1 >> 0 ) & 0xFF ); break;
				case 4: c = (char)( ( dwKey2 >> 24 ) & 0xFF ); break;
				case 5: c = (char)( ( dwKey2 >> 16 ) & 0xFF ); break;
				case 6: c = (char)( ( dwKey2 >> 8 ) & 0xFF ); break;
				case 7: c = (char)( ( dwKey2 >> 0 ) & 0xFF ); break;
				}
				str += p ^ c;
			}
			return str;
		}
	}
}