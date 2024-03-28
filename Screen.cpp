#include "Screen.h"
#include <stdexcept>


Screen::Screen() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Screen::clearScreen() {
	system("cls");
}

void Screen::moveCursor(int x, int y) {
	COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(hConsole, position);
}

std::pair<int, int> Screen::getConsoleSize() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		int consoleHight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		return{ consoleWidth, consoleHight };
	}
	else {
		throw std::runtime_error("can not get console information");
	}
}