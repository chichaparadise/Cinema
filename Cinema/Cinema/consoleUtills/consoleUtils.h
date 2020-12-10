#ifndef CONSOLEUTIL_H
#define CONSOLEUTIL_H

#include "Windows.h"
#include "stdint.h"
#include <string>

using namespace std;

typedef enum _colors
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
}ConsoleColor;

class Console
{
public:
	Console()
	{
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		SetConsoleTitle(L"Кинотеатр");
		this->xSize = 80;
		this->ySize = 25;
		stdoutHndl = GetStdHandle(STD_OUTPUT_HANDLE);
		this->cursor.X = 0;
		this->cursor.Y = 0;
	}

	Console(uint8_t xSize, uint8_t ySize)
	{
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		SetConsoleTitle(L"Кинотеатр");
		this->xSize = xSize;
		this->ySize = ySize;
		char buff[40];
		sprintf(buffer, "mode con cols=%d lines=%d", xSize, ySize); 
		system(buffer);
		stdoutHndl = GetStdHandle(STD_OUTPUT_HANDLE);
		this->cursor.X = 0;
		this->cursor.Y = 0;
	}

	bool SetCursor(int x, int y)
	{
		this->cursor.X = x;
		this->cursor.Y = y;
		return (bool)SetConsoleCursorPosition(this->stdoutHndl, cursor);
	}

	bool SetAttribute(uint8_t bg, uint16_t fg)
	{
		return (bool)SetConsoleTextAttribute(this->stdoutHndl , WORD((bg << 4) | fg));
	}	

	WORD DrawAttributeHorizontalLine(int x, int y, int length, uint8_t bg, uint8_t fg)
	{
		WORD res;
		WORD attr = WORD((bg << 4) | fg);
		COORD crd;
		crd.X = x;
		crd.Y = y;
		FillConsoleOutputAttribute(this->stdoutHndl, attr, length, crd, &res);
		return res;
	}

	void SetCursorInvisible()
	{
		СONSOLE_CURSOR_INFO curs = {0};
   		curs.dwSize = sizeof(curs);
   		curs.bVisible = FALSE;
		SetConsoleCursorInfo(this->stdoutHndl, &curs);
	}

	bool SetCursorVisible()
	{
		СONSOLE_CURSOR_INFO curs = {0};
   		curs.dwSize = sizeof(curs);
   		curs.bVisible = TRUE;
		SetConsoleCursorInfo(this->stdoutHndl, &curs);
	}

	COORD GetCurrentCoords()
	{
		return cursor;
	}
private:
	COORD cursor;

	int xSize;

	int ySize;

	HANDLE stdoutHndl;
};

#endif /*CONSOLENUTIL_H*/