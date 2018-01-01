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

	FillConsoleOutputAttribute(consoleHandle, bufferInfo.wAttributes, totalChars, startPos, &charsWritten); // Resetting attributes
	SetConsoleCursorPosition(consoleHandle, startPos); // Position cursor to home
	std::cout << "Terminal Resetted by emulation";
	return true;
}

auto emulate_clear_inefficient() {
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
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, BACKGROUND_GREEN|FOREGROUND_RED);
	
	std::string s = "Microsoft Corporation (/ˈmaɪkrəˌsɒft/,[6][7] abbreviated as MS) is an American multinational technology company with headquarters in Redmond, Washington.\n It develops, manufactures, licenses, supports and sells computer software, consumer electronics, personal computers, and services.\n Its best known software products are the Microsoft Windows line of operating systems, the Microsoft Office suite, and the Internet Explorer and Edge web browsers.\n Its flagship hardware products are the Xbox video game consoles and the Microsoft Surface tablet lineup.\n As of 2016, it is the world's largest software maker by revenue,[8] and one of the world's most valuable companies.\n[9] The word 'Microsoft' is a portmanteau of 'microcomputer' and 'software'.\n Microsoft was founded by Paul Allen and Bill Gates on April 4, 1975, to develop and sell BASIC interpreters for the Altair 8800.\n It rose to dominate the personal computer operating system market with MS-DOS in the mid-1980s, followed by Microsoft Windows.\n The company's 1986 initial public offering (IPO), and subsequent rise in its share price, created three billionaires and an estimated 12,000 millionaires among Microsoft employees.\n Since the 1990s, it has increasingly diversified from the operating system market and has made a number of corporate acquisitions—their largest being the acquisition of LinkedIn for $26.2 billion in December 2016,[11] followed by Skype Technologies for $8.5 billion in May 2011.[12]\n As of 2015, Microsoft is market - dominant in the IBM PC - compatible operating system market and the office software suite market, although it has lost the majority of the overall operating system market to Android.[13]\n The company also produces a wide range of other consumer and enterprise software for desktops and servers, including Internet search(with Bing), the digital services market(through MSN), mixed reality(HoloLens), cloud computing(Azure) and software development(Visual Studio).\n Steve Ballmer replaced Gates as CEO in 2000, and later envisioned a 'devices and services' strategy.[14]\n This began with the acquisition of Danger Inc.in 2008, [15] entering the personal computer production market for the first time in June 2012 with the launch of the Microsoft Surface line of tablet computers; and later forming Microsoft Mobile through the acquisition of Nokia's devices and services division.\n Since Satya Nadella took over as CEO in 2014, the company has scaled back on hardware and has instead focused on cloud computing, a move that helped the company's shares reach its highest value since December 1999.[16][17]\n";
	std::cout << s;
		
	char bogus;
	std::cin >> bogus;
	
	emulate_clear();

	std::cin >> bogus;

	return 0;
}