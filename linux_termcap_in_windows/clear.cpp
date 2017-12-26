#include <iostream>
#include <string>
#include <windows.h>

auto containsAnythingOtherThanNull(const std::string& s) {
	for (auto&c : s) {
		if (c != '\0' and c != ' ') {
			return true;
		}
	}
	return false;
}

auto emulate_clear() {
	// First, goto last non-null non-space character in the buffer
	// then add multiple '\r\n's according to number of rows in viewport
	// and then re-position the cursor to the first col of line found in step 1
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	std::cout << "hello world\n line some very very very very very very very very long line that should either wrap or it's half not being displayed\n line 2\n line 3\n line 4\n line 5\n line 6\n line 7\n line 8\n line 9\n line 10\n line 11\n line 12\n line 13\n line 14\n line 15\n line 16\n line 17\n line 18\n line 19\n line 20\n line 21\n line 22\n line 23\n line 24\n line 25\n line 26\n line 27\n line 28\n line 29\n line 30\n line 31\n line 32\n";
	CONSOLE_SCREEN_BUFFER_INFO buffInfo;

	if (!GetConsoleScreenBufferInfo(consoleHandle, &buffInfo)) {
		return false;
	}

	DWORD written, read;
	std::string r;
	r.resize(buffInfo.dwSize.X);
	short y = buffInfo.dwSize.Y - 1;
	for (; y >= 0; y--) {
		ReadConsoleOutputCharacter(consoleHandle, &r[0], r.size(), { 0,y }, &read);
		if (containsAnythingOtherThanNull(r)) {
			break;
		}
	}

	if (!SetConsoleCursorPosition(consoleHandle, { 0 , y + 1 })) {
		return false;
	}

	auto viewPortHeight = buffInfo.srWindow.Bottom - buffInfo.srWindow.Top + 1;
	std::string newlines;
	for (auto i = 1; i <= viewPortHeight; i++) {
		newlines += std::string("\r\n");
	}
	std::cout << newlines;
	if (!SetConsoleCursorPosition(consoleHandle, { 0, y + 1 })) {
		return false;
	}
	std::cout << "cleared";
	return true;
}

 int main() {
	emulate_clear();

	char bogus;
	std::cin >> bogus;

	return 0;
 }