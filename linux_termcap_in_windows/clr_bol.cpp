#include <iostream>
#include <string>
#include <windows.h>
#include "move_cursor.h"

auto emulate_clr_bol() {

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	// SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
	
	std::string s = "hello world, i am emulating clr_bol,\n hope this disappears";
	std::cout << s;

	if (!move_cursor(consoleHandle, -10, 0)) {
		// move_cursor returned false
		return false;
	}

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	if (!GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo)) {
		// Error in fetching screen buffer info
		return false;
	}
	
	SMALL_RECT contentToMove = { bufferInfo.dwCursorPosition.X, bufferInfo.dwCursorPosition.Y, bufferInfo.dwSize.X, bufferInfo.dwCursorPosition.Y };
	CHAR_INFO fill{' ', bufferInfo.wAttributes};
	COORD dest{ 0,bufferInfo.dwCursorPosition.Y };

	// TODO: Fix the wrapping of attributes on the same row
	//WORD attr = 0;
	//DWORD written;
	//FillConsoleOutputAttribute(consoleHandle, attr, bufferInfo.dwCursorPosition.X, { 0, bufferInfo.dwCursorPosition.Y }, &written);

	// Move the remaining line to the left
	if (!ScrollConsoleScreenBuffer(consoleHandle, &contentToMove, nullptr, dest, &fill)) {
		// ScrollConsoleScreenBuffer Failed
		return false;
	}

	// Position cursor to beginning of line
	if (!SetConsoleCursorPosition(consoleHandle, dest)) {
		// SetConsoleCursorPosition Failed
		return false;
	}
	return true;
}


int main() {
	emulate_clr_bol();

	char bogus;
	std::cin >> bogus;

	return 0;
}