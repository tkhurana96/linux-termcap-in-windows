#include <iostream>
#include <string>
#include <windows.h>

auto move_cursor(HANDLE consoleHandle, int dx, int dy) {
	// TODO: Handle the case where cursor needs to be moved to previous or next lines given enough dx
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

	if (!GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo)) {
		// Error in fetching screen buffer info
		return false;
	}

	COORD dest = {bufferInfo.dwCursorPosition.X + dx,  bufferInfo.dwCursorPosition.Y + dy};
	if (!SetConsoleCursorPosition(consoleHandle, dest)) {
		return false;
	}
	return true;
}

auto emulate_clr_bol() {

	std::string s = "hello world, i am emulating clr_bol,\n hope this disappears";

	std::cout << s;
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

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