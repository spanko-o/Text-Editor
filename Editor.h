#pragma once

#include "Buffer.h"
#include "Screen.h"
#include <string>

class Editor {
private:
	Buffer buffer;
	Screen screen;
	std::string openFilename;
	int cursorX = 0, cursorY = 0;
public:
	Editor(const std::string& filename);
	void run();
private:
	void handleControlKey(char key);
	void renderScreen();
	void onUpKeyPressed();
	void onDownKeyPressed(int consoleHeight);
	void onRightKeyPressed(int consoleWidth);
	void onLeftKeyPressed();
};