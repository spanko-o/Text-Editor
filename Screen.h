#pragma once

#include <Windows.h>
#include <utility>

class Screen {
private:
	HANDLE hConsole;

public:
	Screen();
	void moveCursor(int x, int y);
	void clearScreen();
	std::pair<int, int> getConsoleSize();
};