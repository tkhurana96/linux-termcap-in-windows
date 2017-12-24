#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>
#include <thread>

auto emulate_cnorm() {
	using namespace std::chrono_literals;
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	std::string s = "hello world, you can't see the cursor, do you ? Now can you see it ?";

	CONSOLE_CURSOR_INFO cursor_hide_props{ 1, false };
	if (!SetConsoleCursorInfo(consoleHandle, &cursor_hide_props)) {
		// SetConsoleCursorInfo failed
		return false;
	}

	CONSOLE_CURSOR_INFO cursor_show_props{ 1, true };

	auto i = 0;
	while (i < s.size()) {
		std::cout << s[i];
		if (i == 48) {
			SetConsoleCursorInfo(consoleHandle, &cursor_show_props);
		}
		std::this_thread::sleep_for(0.05s);
		i++;
	}
	return true;
}


int main() {
	emulate_cnorm();

	char bogus;
	std::cin >> bogus;

	return 0;
}