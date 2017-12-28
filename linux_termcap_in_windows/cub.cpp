#include <iostream>
#include <string>
#include <windows.h>

auto emulate_cub(const short jump = 1) {
	if (jump > 0) {
		auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO buffInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
		short destCol = max(buffInfo.srWindow.Left, buffInfo.dwCursorPosition.X - jump);
		SetConsoleCursorPosition(consoleHandle, { destCol, buffInfo.dwCursorPosition.Y });
	}
}

int main() {
	std::string s("hello world\n some very very very very very very very very long line that should either wrap or it's half not being displayed\n line 2\n line 3\n line 4\n line 5\n line 6\n line 7\n line 8\n line 9\n line 10\n line 11\n line 12\n line 13\n line 14\n line 15\n line 16\n line 17\n line 18\n line 19\n line 20\n line 21\n line 22\n line 23\n line 24\n line 25\n line 26\n line 27\n line 28\n line 29\n line 30\n line 31\n line 32");
	std::cout << s;

	emulate_cub(5);
	std::cout << "THIS THING IS NEWLY WRITTEN";

	char bogus;
	std::cin >> bogus;

	return 0;
}