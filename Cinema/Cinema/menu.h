#ifndef MENU_H
#define MENU_H

#include "consoleUtills\consoleUtils.h"

using namespace std;

typedef enum _keys_t
{
	UpArrow = 72,
	DownArrow = 80,
	RightArrow = 77,
	LeftArrow = 75,
	Esc = 27,
	Enter = 13,
	Delete = 83
}Keys;

class Menu
{
public:
	Menu()
	{
		console = Console();
	}

	Menu(uint8_t xSize, uint8_t ySize)
	{
		console = Console(xSize, ySize);

	}

	
private:
	Console console;
};


#endif
