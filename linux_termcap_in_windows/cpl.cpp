#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

auto emulate_cpl(const short jump) {
	if (jump > 0) {
		auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO buffInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
		short destRow = max(buffInfo.srWindow.Top, buffInfo.dwCursorPosition.Y - jump);
		SetConsoleCursorPosition(consoleHandle, { buffInfo.srWindow.Left, destRow });
	}
}

int main() {

	std::string s("hello world\n some very very very very very very very very long line that should either wrap or it's half not being displayed\n line 2\n line 3\n line 4\n line 5\n line 6\n line 7\n line 8\n line 9\n line 10\n line 11\n line 12\n line 13\n line 14\n line 15\n line 16\n line 17\n line 18\n line 19\n line 20\n line 21\n line 22\n line 23\n line 24\n line 25\n line 26\n line 27\n line 28\n line 29\n line 30\n line 31\n line 32");
	std::cout << s;

	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(consoleHandle, { 10, 40 });

	for (short i = 1; i <= 20; i++) {
		emulate_cpl(i);
		std::cout << "Jumped by " << i;
		std::this_thread::sleep_for(1s);
	}

	char bogus;
	std::cin >> bogus;

	return 0;
}