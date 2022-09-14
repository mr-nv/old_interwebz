#ifndef _DRAW_H
#define _DRAW_H

typedef unsigned long HFont;

class cDraw
{
public:
	HFont espFont,consoleFont;
	int getHeight();
	int getWidht(const char*);
	std::wstring stringToWide(const std::string& text);
	void initFont(int fontsize,const char* font);
	void drawString(bool center,int x, int y, int r,int g, int b, const char *input, ...);

private:
	
};

#endif