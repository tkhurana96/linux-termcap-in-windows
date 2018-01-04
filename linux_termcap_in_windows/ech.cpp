#include <iostream>
#include <windows.h>
#include <string>

auto emulate_ech(short numChars = 1) {
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (numChars > 0) {
		CONSOLE_SCREEN_BUFFER_INFO buffInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
		auto remainingChars = buffInfo.dwSize.X - buffInfo.dwCursorPosition.X;
		numChars = min(numChars, remainingChars);
		DWORD written;
		FillConsoleOutputCharacter(consoleHandle, CHAR(' '), numChars, buffInfo.dwCursorPosition, &written);
		FillConsoleOutputAttribute(consoleHandle, buffInfo.wAttributes, numChars, buffInfo.dwCursorPosition, &written);
	}

}

auto actual_ech(const short numChars = 1) {
	DWORD dwMode = 0;
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(consoleHandle, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(consoleHandle, dwMode);
	std::cout << "\x1b[" << numChars << "X";
}

int main() {
	std::string s("$line 1\n$line 2\n$line 3\n$line 4");
	std::cout << s;

	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
	GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
	SetConsoleCursorPosition(consoleHandle, { 2, buffInfo.dwCursorPosition.Y - 2 });
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);

	emulate_ech(10);
	// actual_ech(10);

	char bogus;
	std::cin >> bogus;
	return 0;
}