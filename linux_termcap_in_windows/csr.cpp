#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>


class ScrollRegion {
private:
	SMALL_RECT area;
	CHAR_INFO attr{' ', FOREGROUND_GREEN|FOREGROUND_INTENSITY|BACKGROUND_RED};
	bool initialized = false;

	auto init(HANDLE consoleHandle) {
		DWORD written;
		for (short row = area.Top; row <= area.Bottom; row++) {
			initialized = FillConsoleOutputAttribute(consoleHandle, attr.Attributes, area.Right - area.Left + 1, { area.Left, row }, &written);
		}
		return initialized;
	}
public:
	ScrollRegion(SMALL_RECT rect) : area(rect) {}
	// ScrollRegion(const ScrollRegion&) = delete;

	auto includes(COORD point) const {
		if (area.Left <= point.X and point.X <= area.Right) {
			if (area.Top <= point.Y and point.Y <= area.Bottom) {
				return true;
			}
		}
		return false;
	}

	auto write(HANDLE consoleHandle, std::string text) {
		// manage all writing and overflows and all
		if (!initialized) {
			init(consoleHandle);
		}

		DWORD written;
		SMALL_RECT toMove{ area.Left, area.Top + 1 , area.Right, area.Bottom };
		ScrollConsoleScreenBuffer(consoleHandle, &toMove, nullptr, { area.Left, area.Top }, &attr);
		COORD cursorDest{ area.Left, area.Bottom };
		SetConsoleCursorPosition(consoleHandle, cursorDest);
		FillConsoleOutputAttribute(consoleHandle, attr.Attributes, text.size(), cursorDest, &written);
		WriteConsoleOutputCharacter(consoleHandle, text.data(), text.size(), cursorDest, &written);
	}

	auto overlaps(const ScrollRegion& r) const {
		COORD topLeft{ r.area.Left, r.area.Top };
		COORD topRight{ r.area.Right, r.area.Top };
		COORD bottomLeft{ r.area.Left, r.area.Bottom };
		COORD bottomRight{ r.area.Right, r.area.Bottom };

		return !(includes(topLeft) || includes(topRight) || includes(bottomLeft) || includes(bottomRight));
	}
};

class Console {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	std::vector<ScrollRegion> regions;
public:
	Console(const Console&) = delete;
	Console() {}

	auto write(std::string, COORD);
	auto addRegion(SMALL_RECT);
};

auto Console::write(std::string text, COORD startPt) {
	for (auto& each_region : regions) {
		if (each_region.includes(startPt)) {
			each_region.write(consoleHandle, text);
			return true;
		}
	}
	return false;
}

auto Console::addRegion(SMALL_RECT r) {
	ScrollRegion newRegion(r);
	for (auto& region : regions) {
		// if newRegion overlaps with any other region then return false
		if (region.overlaps(newRegion) || newRegion.overlaps(region)) {
			return false;
		}
	}
	regions.push_back(newRegion);
	return true;
}

int main() {
	using namespace std::chrono_literals;
	Console c;

	c.addRegion({ 10, 10, 20, 20 });

	auto numStars = 1;
	while (true){
		c.write(std::string(numStars, '*'), { 11, 11 });
		numStars = ((numStars + 1) % 5) + 1;
		std::this_thread::sleep_for(0.10s);
	}

	return 0;
}