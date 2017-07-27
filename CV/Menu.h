#pragma once
#include <Windows.h>
#include <fstream>
#include <functional>
#define MAX_MENU_OPTIONS 16
#define CLEAR_SCREEN_CHAR ' '
#define MAX_CHARS_PER_LINE 89
#define DESC_SPLIT_CHAR ';'
#define STARTING_COLUMN 27
#define STARTING_ROW 3

typedef unsigned int uint;
typedef short s_int;

enum Colors : WORD{
	BLACK = 0x0,
	GREY = 0x8,
	RED = 0x4,
	LIGHT_RED = 0xC,
	GREEN = 0x2,
	LIGHT_GREEN = 0xA,
	AQUA = 0x3,
	LIGHT_AQUA = 0x0B,
	BLUE = 0x1,
	LIGHT_BLUE = 0x9,
	PURPLE = 0x5,
	LIGHT_PURPLE = 0xD,
	YELLOW = 0x6,
	LIGHT_YELLOW = 0xE,
	WHITE = 0x7,
	BRIGHT_WHITE = 0xF,
};

enum Menu_State {
	MAIN_MENU,
	DISPLAYING
};

struct Menu_Option {
	const char* title_string,
		*content_string;
	uint option_number;
	uint selected = false, active = false;
	std::function<void(void)> callback;	//Function called when the option is selected
};

class Menu
{
private:
	uint current_option = 0;
	uint num_options = 0;
	Menu_State state = MAIN_MENU;

	const WORD fg_color[2] = {
		BRIGHT_WHITE, BLACK
	};
	const WORD bg_color[2] = {
		BLACK, BRIGHT_WHITE
	};

	Menu_Option options[MAX_MENU_OPTIONS];

	HANDLE console_output_handler;
	HANDLE input_handler;

	void MoveCursor(s_int x, s_int y);
	void setColor(uint foreground = 0x0, uint background = 0x7);
	void ClearConsole();

public:

	Menu();
	bool AddOption(const char* option, const char* content, std::function<void(void)> callback = nullptr);
	bool CheckInput();
	void Print();
	void Free();
	virtual ~Menu();
};

