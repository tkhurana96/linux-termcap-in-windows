﻿#include <iostream>
#include <string>
#include <windows.h>

auto do_actual_clear() {
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(consoleHandle, &dwMode);

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(consoleHandle, dwMode);

	std::cout << "\x1b[2J";
	std::cout << "Terminal Resetted by ansi";
}

auto emulate_reset() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

	if (!GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo)) {
		std::cout << "Error in fetching screen buffer info\n";
		return false;
	}

	std::cout << "old attrs are: " << bufferInfo.wAttributes << '\n';	

	DWORD totalChars = bufferInfo.dwSize.X * bufferInfo.dwSize.Y;
	COORD startPos = { 0, 0 };
	DWORD charsWritten = 0;

	if (!FillConsoleOutputCharacter(consoleHandle, ' ', totalChars, startPos, &charsWritten)) {
		std::cout << "Error in clearing the screen\n";
		return false;
	 }

	FillConsoleOutputAttribute(consoleHandle, FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED, totalChars, startPos, &charsWritten); // Resetting attributes
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	SetConsoleCursorPosition(consoleHandle, startPos); // Position cursor to home
	std::cout << "Terminal Resetted by emulation";
	return true;
}

int main() {
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, BACKGROUND_GREEN|FOREGROUND_RED);

	std::string s = "Microsoft Corporation (/ˈmaɪkrəˌsɒft/,[6][7] abbreviated as MS) is an American multinational technology company with headquarters in Redmond, Washington.\n It develops, manufactures, licenses, supports and sells computer software, consumer electronics, personal computers, and services.\n Its best known software products are the Microsoft Windows line of operating systems, the Microsoft Office suite, and the Internet Explorer and Edge web browsers.\n Its flagship hardware products are the Xbox video game consoles and the Microsoft Surface tablet lineup.\n As of 2016, it is the world's largest software maker by revenue,[8] and one of the world's most valuable companies.\n[9] The word 'Microsoft' is a portmanteau of 'microcomputer' and 'software'.\n Microsoft was founded by Paul Allen and Bill Gates on April 4, 1975, to develop and sell BASIC interpreters for the Altair 8800.\n It rose to dominate the personal computer operating system market with MS-DOS in the mid-1980s, followed by Microsoft Windows.\n The company's 1986 initial public offering (IPO), and subsequent rise in its share price, created three billionaires and an estimated 12,000 millionaires among Microsoft employees.\n Since the 1990s, it has increasingly diversified from the operating system market and has made a number of corporate acquisitions—their largest being the acquisition of LinkedIn for $26.2 billion in December 2016,[11] followed by Skype Technologies for $8.5 billion in May 2011.[12]\n As of 2015, Microsoft is market - dominant in the IBM PC - compatible operating system market and the office software suite market, although it has lost the majority of the overall operating system market to Android.[13]\n The company also produces a wide range of other consumer and enterprise software for desktops and servers, including Internet search(with Bing), the digital services market(through MSN), mixed reality(HoloLens), cloud computing(Azure) and software development(Visual Studio).\n Steve Ballmer replaced Gates as CEO in 2000, and later envisioned a 'devices and services' strategy.[14]\n This began with the acquisition of Danger Inc.in 2008, [15] entering the personal computer production market for the first time in June 2012 with the launch of the Microsoft Surface line of tablet computers; and later forming Microsoft Mobile through the acquisition of Nokia's devices and services division.\n Since Satya Nadella took over as CEO in 2014, the company has scaled back on hardware and has instead focused on cloud computing, a move that helped the company's shares reach its highest value since December 1999.[16][17]\n";
	std::cout << s;
	
	char bogus;
	std::cin >> bogus;

	emulate_reset();
	// do_actual_clear();
	
	std::cin >> bogus;

	return 0;
}