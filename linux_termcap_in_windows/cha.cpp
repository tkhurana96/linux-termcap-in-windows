#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

auto emulate_cha(const short destCol) {
	if (destCol > 0) {
		auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO buffInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);
		if (buffInfo.srWindow.Left <= destCol and destCol <= buffInfo.srWindow.Right) {
			SetConsoleCursorPosition(consoleHandle, { buffInfo.srWindow.Left + destCol - 1, buffInfo.dwCursorPosition.Y });
		}
	}
}

int main() {

	for (auto i = 1, j = 100; i <50, j >50; i++, j--) {
		emulate_cha(i);
		std::cout << '*';

		emulate_cha(j);
		std::cout << '$';

		std::this_thread::sleep_for(0.05s);
	}
	char bogus;
	std::cin >> bogus;
	return 0;
}