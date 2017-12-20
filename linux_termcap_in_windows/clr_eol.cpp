#include <iostream>
#include <windows.h>
#include <string>
#include "move_cursor.h"

auto emulate_clr_eol() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
	std::string s = "hello world, I am emulating clr_eol, hope this disappears";
	std::cout << s;

	// some cursor moving task
	if (!move_cursor(consoleHandle, -10, 0)) {
		// move_cursor returned false
		return false;
	}

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	if (!GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo)) {
		// Error in fetching screen buffer info
		return false;
	}

	DWORD written, numChars = bufferInfo.dwSize.X - bufferInfo.dwCursorPosition.X + 1;
	if (!FillConsoleOutputCharacter(consoleHandle, ' ', numChars, bufferInfo.dwCursorPosition, &written)) {
		// FillConsoleOutputCharacter Failed
		return false;
	}

	// TODO: Fix the remaining attributes on screen
	// FillConsoleOutputAttribute(consoleHandle, , numChars, bufferInfo.dwCursorPosition, written);
	return true;
}

int main() {
	emulate_clr_eol();

	char bogus;
	std::cin >> bogus;

	return 0;
}