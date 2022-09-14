#include "Includes.h"

void cDraw::initFont(int fontsize,const char* font)
{
	espFont = g_pSurface->CreateFont();
	g_pSurface->SetFontGlyphSet(espFont,font,fontsize,FW_DONTCARE,0,0,0x200);
}

std::wstring cDraw::stringToWide(const std::string& text)
{
	std::wstring wide( text.length(), L' ' );
	std::copy( text.begin(), text.end(), wide.begin() );

	return wide;
}

int cDraw::getWidht(const char* input)
{
	INT iWide = 0;
	INT iTall = 0;
	INT iBufSize = MultiByteToWideChar ( CP_UTF8, 0x0, input, -1, NULL, 0 );

	wchar_t* pszUnicode = new wchar_t [ iBufSize ];

	MultiByteToWideChar ( CP_UTF8, 0x0, input, -1, pszUnicode, iBufSize );

	g_pSurface->GetTextSize ( espFont, pszUnicode, iWide, iTall );

	delete [ ] pszUnicode;

	return iWide;
}

void cDraw::drawString(bool center,int x, int y, int r,int g, int b, const char *input, ...)
{
	CHAR szBuffer [ MAX_PATH ];

	if ( !input )
		return;

	vsprintf ( szBuffer, input, ( char* ) &input + _INTSIZEOF ( input ) );


	if ( center )
		x -= getWidht ( szBuffer ) / 2;

	g_pSurface->DrawSetTextColor ( r, g, b, 255 );
	g_pSurface->DrawSetTextFont ( espFont );
	g_pSurface->DrawSetTextPos ( x, y );

	std::wstring wide = stringToWide( std::string( szBuffer ) );
	g_pSurface->DrawPrintText ( wide.c_str(), wide.length() );
}