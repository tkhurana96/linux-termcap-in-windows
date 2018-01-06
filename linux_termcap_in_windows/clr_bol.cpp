#include <iostream>
#include <string>
#include <windows.h>

auto actual_clr_bol() {
	DWORD dwMode = 0;
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(consoleHandle, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(consoleHandle, dwMode);
	std::cout << "\x1b[1K";
}


auto emulate_clr_bol() {

	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);

	COORD spacesStartPoint{ 0, buffInfo.dwCursorPosition.Y};
	DWORD written, numSpaces = buffInfo.dwCursorPosition.X + 1;

	FillConsoleOutputCharacter(consoleHandle, ' ', numSpaces, spacesStartPoint, &written);
	FillConsoleOutputAttribute(consoleHandle, buffInfo.wAttributes, numSpaces, spacesStartPoint, &written);
}


int main() {

	for (auto i = 1; i <= 50; i++) {
		std::cout << "$line " << i << '\n';
	}
	
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
	SetConsoleCursorPosition(consoleHandle, { 5, buffInfo.dwCursorPosition.Y - 25 });
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);

	emulate_clr_bol();
	// actual_clr_bol();
	
	char bogus;
	std::cin >> bogus;

	return 0;
}