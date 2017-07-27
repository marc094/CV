#include "Menu.h"
#define _FILEPATH "CV.txt"
#define _MAX_BUFFER_SIZE 2048
#define TITLE_CHAR '#'
#define DESC_CHAR '*'
#define COMMENT_CHAR '>'

enum ERR_TYPE {
	NO_ERR,
	READ_ERR,
	FORMAT_ERR,

	ERR_COUNT
};

struct Error {
	ERR_TYPE type = NO_ERR;
	char description[ERR_COUNT][_MAX_BUFFER_SIZE] = {	//Can't free those strings
		"\nError: No error",
		"\nError: Couldn't open file: ",
		"\nError: Couldn't read file, wrong format: "
	};
} error;

bool loadFile(const char* file_path, Menu* menu) {
	error.type = NO_ERR;
	char* buffer = (char*)calloc(_MAX_BUFFER_SIZE, sizeof(char));
	uint option_index = 0;
	std::fstream file;

	file.open(file_path, std::ios_base::in);
	if (!file.good()) {
		error.type = READ_ERR;
		strcat_s(error.description[error.type], file_path);
		return false;
	}

	char* content = nullptr, *title = nullptr;
	while (!file.eof()) {
		file.get(buffer, 2);
		switch (buffer[0]) {
		case TITLE_CHAR:
			title = (char*)calloc(_MAX_BUFFER_SIZE, sizeof(char));
			file.getline(title, _MAX_BUFFER_SIZE);
			break;
		case DESC_CHAR:
			content = (char*)calloc(_MAX_BUFFER_SIZE, sizeof(char));
			file.getline(content, _MAX_BUFFER_SIZE);
			menu->AddOption(title, content);
			break;
		case COMMENT_CHAR:
			file.getline(buffer, _MAX_BUFFER_SIZE);
			break;
		case '\0':
			file.read(buffer, 2);
			break;
		default:
			error.type = FORMAT_ERR;
			strcat_s(error.description[error.type], file_path);
			return false;
		}
	}

	file.close();
	return true;
}

void PrintError() {
	printf(error.description[error.type]);
	getchar();
}

void main()
{
	bool end = false;
	Menu menu;

	if (!loadFile(_FILEPATH, &menu)) {
		PrintError();
	} else {
		while (!end) {
			menu.Print();
			end = menu.CheckInput();
		}

		menu.Free();
	}
	return;
}