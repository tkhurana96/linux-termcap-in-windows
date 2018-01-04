#include <iostream>
#include <windows.h>
#include <string>

auto emulate_indn(short rowsToScroll = 1) {
	if (rowsToScroll > 0) {
		auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO buffInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
		auto totalRows = buffInfo.srWindow.Bottom - buffInfo.srWindow.Top;
		rowsToScroll = min(rowsToScroll, totalRows);
		short firstRowOfContentToMove = buffInfo.srWindow.Top + rowsToScroll;

		SMALL_RECT contentToMove{ buffInfo.srWindow.Left, firstRowOfContentToMove, buffInfo.srWindow.Right, buffInfo.srWindow.Bottom + 1};

		CHAR_INFO charToFill{ CHAR(' '), buffInfo.wAttributes };
		ScrollConsoleScreenBuffer(consoleHandle, &contentToMove, nullptr, { buffInfo.srWindow.Left, buffInfo.srWindow.Top }, &charToFill);
	}
}

auto actual_indn(const short rowsToScroll = 1) {
	DWORD dwMode = 0;
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(consoleHandle, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(consoleHandle, dwMode);
	std::cout << "\x1b[" << rowsToScroll << "S";
}

//int main() {
//
//	char bogus;
//	std::string s(" line 1\n line 2\n line 3\n line 4\n line 5\n line 6\n line 7\n line 8\n line 9\n line 10\n line 11\n line 12\n line 13\n line 14\n line 15\n line 16\n line 17\n line 18\n line 19\n line 20\n line 21\n line 22\n line 23\n line 24\n line 25\n line 26\n line 27\n line 28\n line 29\n line 30\n line 31\n line 32\n line 33\n line 34\n line 35\n line 36\n line 37");
//
//	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
//	CONSOLE_SCREEN_BUFFER_INFO buffInfo;
//	SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
//
//	std::cout << s;
//	GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
//	SetConsoleCursorPosition(consoleHandle, { buffInfo.dwCursorPosition.X ,buffInfo.dwCursorPosition.Y - 32 });
//	
//	emulate_indn(5);
//	// actual_indn(5);
//
//	std::cin >> bogus;
//	return 0;
//}