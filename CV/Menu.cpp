#include "Menu.h"

Menu::Menu()
{
	console_output_handler = GetStdHandle(STD_OUTPUT_HANDLE);
	input_handler = GetStdHandle(STD_INPUT_HANDLE);
}


Menu::~Menu()
{
}

void Menu::MoveCursor(s_int x, s_int y) {
	SetConsoleCursorPosition(console_output_handler, {x, y});
}


void Menu::setColor(uint foreground, uint background) {
	background = background << 4;
	uint final_color = background | foreground;
	SetConsoleTextAttribute(console_output_handler, final_color);
}

bool Menu::AddOption(const char* option, const char* content, std::function<void(void)> callback) {
	if (num_options >= MAX_MENU_OPTIONS) return false;	//False is returned in case the max amount of options is already reached
	options[num_options].title_string = option;
	options[num_options].content_string = content;
	options[num_options].active = true;
	options[num_options].selected = false;
	options[num_options].option_number = num_options;
	options[num_options].callback = callback;
	num_options++;
	return true;
}

void Menu::Print() {
	uint x = 2, y = 1;
	MoveCursor(x, y);
	setColor(fg_color[0], bg_color[0]);
	printf("Choose an option:\n");
	for (uint i = 0; i < MAX_MENU_OPTIONS; i++) {
		if (!options[i].active) return;
		setColor(fg_color[current_option == i], bg_color[current_option == i]);
		y += 2;
		MoveCursor(x, y);
		printf(options[i].title_string);

		if (options[i].selected) {
			setColor(fg_color[0], bg_color[0]);
			uint char_counter = 0, lines = 0;
			while (options[i].content_string[char_counter] != '\0') {
				char *string = (char*)calloc(MAX_CHARS_PER_LINE + 1, sizeof(char));
				uint j = 0;
				MoveCursor(STARTING_COLUMN, STARTING_ROW + lines);
				while (j < MAX_CHARS_PER_LINE && options[i].content_string[char_counter] != '\0') {
					if (options[i].content_string[char_counter] == DESC_SPLIT_CHAR) {
						char_counter++;
						lines++;
						break;
					}
					string[j] = options[i].content_string[char_counter];
					j++;
					char_counter++;
				}
				printf(string);
				lines++;
				delete[] string;
			}
		}
	}
}

bool Menu::CheckInput() {

	bool Continue = true;
	INPUT_RECORD InRec;
	DWORD NumRead;


	while (Continue)
	{
		ReadConsoleInput(input_handler, &InRec, 1, &NumRead);

		switch (InRec.EventType)
		{
		case KEY_EVENT:
			if (InRec.Event.KeyEvent.bKeyDown) continue;
			switch (InRec.Event.KeyEvent.wVirtualKeyCode) {
			case VK_UP:
				if (state == MAIN_MENU) {
					if (current_option < 1) current_option = num_options;
					current_option--;
					Continue = false;
				}
				break;
			case VK_DOWN:
				if (state == MAIN_MENU) {
					if (current_option >= num_options - 1) current_option = ~0;
					current_option++;
					Continue = false;
				}
				break;
			case VK_ESCAPE:
				return true;
			case VK_RETURN:
				Continue = false;
				if (state == MAIN_MENU) {
					options[current_option].selected = true;
					state = DISPLAYING;
				}
				else if (state == DISPLAYING) {
					options[current_option].selected = false;
					state = MAIN_MENU;
					ClearConsole();
				}
				break;
			}
			break;
		}
	}
	return false;
}

void Menu::ClearConsole() {
	COORD start = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(console_output_handler, &buffer_info);
	DWORD est_length = buffer_info.dwSize.X * buffer_info.dwSize.Y,
		final_length;
	FillConsoleOutputCharacter(console_output_handler, CLEAR_SCREEN_CHAR, est_length, start, &final_length);
	FillConsoleOutputAttribute(console_output_handler, buffer_info.wAttributes, est_length, start, &final_length);
}

void Menu::Free() {
	for (uint i = 0; i < MAX_MENU_OPTIONS; i++) {
		if (!options[i].active) return;
		delete[] options[i].content_string;
		delete[] options[i].title_string;
	}
}