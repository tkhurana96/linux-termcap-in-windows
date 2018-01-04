#include <iostream>
#include <string>
#include <windows.h>

auto emulate_ich(short numSpaces = 1) {
	if (numSpaces > 0) {
		auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO buffInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
	
		auto remainingChars = buffInfo.dwSize.X - buffInfo.dwCursorPosition.X;
		numSpaces = min(numSpaces, remainingChars);
		short numCharsToMove = remainingChars - numSpaces;
	
		SMALL_RECT contentToMove{ buffInfo.dwCursorPosition.X , buffInfo.dwCursorPosition.Y , buffInfo.dwCursorPosition.X + numCharsToMove, buffInfo.dwCursorPosition.Y };
		CHAR_INFO charToFill{ CHAR(' '), buffInfo.wAttributes };
		ScrollConsoleScreenBuffer(consoleHandle, &contentToMove, nullptr, { buffInfo.dwCursorPosition.X + numSpaces, buffInfo.dwCursorPosition.Y }, &charToFill);
	
		DWORD written;
		FillConsoleOutputCharacter(consoleHandle, ' ', numSpaces, buffInfo.dwCursorPosition, &written);
		FillConsoleOutputAttribute(consoleHandle, buffInfo.wAttributes, numSpaces, buffInfo.dwCursorPosition, &written);
	}
}

auto actual_ich(const short numSpaces = 1) {
	DWORD dwMode = 0;
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(consoleHandle, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(consoleHandle, dwMode);
	std::cout << "\x1b[" << numSpaces << '@';
}

int main() {
	char bogus;
	std::string s("$line 1\n$line 2");
	std::cout << s;

	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
	SetConsoleCursorPosition(consoleHandle, { 0, buffInfo.dwCursorPosition.Y });
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
	
	emulate_ich(10);
	// actual_ich(10);
	
	std::cin >> bogus;
	return 0;
}