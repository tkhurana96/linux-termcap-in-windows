#include <iostream>
#include <string>
#include <windows.h>

auto emulate_dl(short numLines = 1) {
	if (numLines > 0) {
		auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO buffInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
		auto remainingLines = buffInfo.srWindow.Bottom - buffInfo.dwCursorPosition.Y + 1;
		numLines = min(remainingLines, numLines);
		auto numRowsToMove = remainingLines - numLines;

		if (numRowsToMove > 0) {
			SMALL_RECT contentToMove{ 0, buffInfo.dwCursorPosition.Y + numLines, buffInfo.dwSize.X,  buffInfo.srWindow.Bottom };
			CHAR_INFO charToFill{ CHAR(' '), buffInfo.wAttributes };
			ScrollConsoleScreenBuffer(consoleHandle, &contentToMove, nullptr, { 0, buffInfo.dwCursorPosition.Y}, &charToFill);
		}

		COORD spacesStartPoint{ 0, buffInfo.dwCursorPosition.Y + numRowsToMove};
		DWORD written, numSpaces = numLines * buffInfo.dwSize.X;
		FillConsoleOutputCharacter(consoleHandle, ' ', numSpaces, spacesStartPoint, &written);
		FillConsoleOutputAttribute(consoleHandle, buffInfo.wAttributes, numSpaces, spacesStartPoint, &written);

		SetConsoleCursorPosition(consoleHandle, { 0, buffInfo.dwCursorPosition.Y });
	}
}

auto actual_dl(short numLines = 1) {
	DWORD dwMode = 0;
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(consoleHandle, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(consoleHandle, dwMode);
	std::cout << "\x1b[" << numLines << "M";
}

int main() {
	for (int i = 1; i <= 50; i++) {
		std::cout << "$line " << i << "\n";
	}

	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
	SetConsoleCursorPosition(consoleHandle, { 5, buffInfo.dwCursorPosition.Y - 25 });
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);

	emulate_dl(10);
	// actual_dl(10);

	char bogus;
	std::cin >> bogus;

	return 0;
}