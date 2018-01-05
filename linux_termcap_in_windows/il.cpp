#include <iostream>
#include <windows.h>
#include <string>

auto emulate_il(short numLines = 1) {
	if (numLines > 0) {
		auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO buffInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
		auto remainingLines = buffInfo.srWindow.Bottom - buffInfo.dwCursorPosition.Y + 1;
		numLines = min(remainingLines, numLines);
		auto numRowsToMove = remainingLines - numLines;

		COORD spacesStartPoint{ 0, buffInfo.dwCursorPosition.Y };
		DWORD written, numSpaces;

		if (numRowsToMove > 0) {
			SMALL_RECT contentToMove{ 0, buffInfo.dwCursorPosition.Y , buffInfo.dwSize.X,  buffInfo.dwCursorPosition.Y + numRowsToMove - 1 };
			CHAR_INFO charToFill{ CHAR(' '), buffInfo.wAttributes };
			ScrollConsoleScreenBuffer(consoleHandle, &contentToMove, nullptr, { 0, buffInfo.dwCursorPosition.Y + numLines }, &charToFill);
		}
		
		numSpaces = numLines * buffInfo.dwSize.X;
		FillConsoleOutputCharacter(consoleHandle, ' ', numSpaces, spacesStartPoint, &written);
		FillConsoleOutputAttribute(consoleHandle, buffInfo.wAttributes, numSpaces, spacesStartPoint, &written);
		
		SetConsoleCursorPosition(consoleHandle, spacesStartPoint);
	}
}

auto actual_il(short numLines = 1) {
	DWORD dwMode = 0;
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(consoleHandle, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(consoleHandle, dwMode);
	std::cout << "\x1b[" << numLines << "L";
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

	emulate_il(10);
	// actual_il(10);

	char bogus;
	std::cin >> bogus;

	return 0;
}
