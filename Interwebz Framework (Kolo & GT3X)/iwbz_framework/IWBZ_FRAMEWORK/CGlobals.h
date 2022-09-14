class CGlobals
{
public:
	//Progressbar
	HWND hProgress;

	HFONT hFont;

	VOID initFont()
	{
		hFont = CreateFontHidden(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, /*calibri*/XorStr<0x12, 8, 0x5FDD4397>("\x71\x72\x78\x7C\x74\x65\x71" + 0x5FDD4397).s);
	}


private:
}; extern CGlobals g_Globals;