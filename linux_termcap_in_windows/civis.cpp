#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>
#include <thread>

auto emulate_civis() {
	using namespace std::chrono_literals;
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	std::string s = "hello world, the cursor will disappear while emulating civis, do you see it ?";

	CONSOLE_CURSOR_INFO cursor_props{ 1, false };
	if (!SetConsoleCursorInfo(consoleHandle, &cursor_props)) {
		// SetConsoleCursorInfo failed
		return false;
	}

	auto i = 0;
	while (i < s.size()) {
		std::cout << s[i];
		std::this_thread::sleep_for(0.05s);
		i++;
	}
	return true;
}


int main() {
	emulate_civis();
	
	char bogus;
	std::cin >> bogus;

	return 0;
}