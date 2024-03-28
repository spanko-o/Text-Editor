#include "Buffer.h"
#include "FileHandler.h"
#include "Screen.h"
#include "Editor.h"
#include <iostream>
#include <conio.h>

const int tabWidth = 8;

Editor::Editor(const std::string& filename) : openFilename(filename) {
	FileHandler::ReadFileIntoBuffer(openFilename, buffer);
	buffer.iter = buffer.text.begin();
	renderScreen();
}

void Editor::run() {
	bool isRunning = true;
	while (isRunning) {
		if (_kbhit()) {
			char ch = _getch(); //获取按键
			//if (ch == 0 || ch == 224) { //特殊按键引导码
			//	ch = _getch();//获取真正键码
			handleControlKey(ch);//处理方向键和backspace
			//}
			//else if (isprint(ch)) {
			//	processPrintableChar(ch);
			//}
			//else /*if (esc)*/ {
			//	isRunning = false;
			//}
		}
	}
}

void Editor::renderScreen() {
	screen.clearScreen();
	std::string content = buffer.getText();
	std::cout << content;
	screen.moveCursor(cursorX, cursorY);
}

void Editor::handleControlKey(char key) {
	auto [consoleWidth, consoleHeight] = screen.getConsoleSize();
	switch (key) {
	case 72: // VK_UP
		onUpKeyPressed();
		break;
	case 80: // VK_DOWN
		onDownKeyPressed(consoleHeight);
		break;
	case 75: // VK_LEFT
		onLeftKeyPressed();
		break;
	case 77: // VK_RIGHT
		onRightKeyPressed(consoleWidth);
		break;
	}
	screen.moveCursor(cursorX, cursorY);
}

void Editor::onUpKeyPressed() {
	if (cursorY > 0) {
		auto lineStart = buffer.iter;
		auto lineEnd = buffer.iter;
		//找到当前行的开始
		while (lineStart != buffer.text.begin() && *prev(lineStart) != '\n') {
			--lineStart;
		}
		//如果当前行不是文件第一行，向上找到一行
		if (lineStart != buffer.text.begin()) {
			cursorY--;
			--lineStart;//上一行换行符
			lineEnd = lineStart;
			//找到上一行开始
			while (lineStart != buffer.text.begin() && *prev(lineStart) != '\n') {
				--lineStart;
			}
		}

		//计算当前行长度
		int charNum = 0;
		int tabAdjustedLength = 0;
		for (auto it = lineStart; it != lineEnd; it++) {
			if (*it == '\t') {
				int nextTabStop = tabWidth - (tabAdjustedLength % tabWidth);
				charNum++;
				tabAdjustedLength += nextTabStop;
			}
			else {
				++charNum;
				++tabAdjustedLength;
			}
		}
		//更新iter到上一行的cursorX位置，如果当前位置在制表符中间，移动到制表符末尾
		int adjustedCursorX = 0;
		buffer.iter = lineStart;
		for (int x = 0; x < charNum; x++) {
			if (adjustedCursorX >= cursorX) { //如果光标已经移动或超过了预期位置（简单说明：cursorX的位置右移是要移动到制表符位置）
				break;
			}
			if (*buffer.iter == '\t') {
				int nextTabStop = tabWidth - (adjustedCursorX % tabWidth);
				if (adjustedCursorX + nextTabStop > cursorX) {
					adjustedCursorX += nextTabStop;
					++buffer.iter;//这里有问题！
					break;
				}
				else {
					adjustedCursorX += nextTabStop;
				}
			}
			else {
				++adjustedCursorX;
			}
			++buffer.iter;
		}
		//如果上一行长度小于当前cursorX，移动光标到上一行末尾
		if (tabAdjustedLength < cursorX) {
			cursorX = tabAdjustedLength;
		}
		else {
			cursorX = adjustedCursorX;
		}
	}
}



void Editor::onDownKeyPressed(int consoleHeight) {
	if (cursorY < consoleHeight - 1) {
		auto tempIt = buffer.iter;
		//找到当前行的行尾
		while (tempIt != buffer.text.end() && *tempIt != '\n') {
			++tempIt;
		}
		//如果行尾不是文件最后一个字符，移动到下一行
		if (tempIt != buffer.text.end()) {
			++tempIt;//跳过换行符
			cursorY++;
			//计算下一行长度
			int nextLineLength = 0;
			int tabAdjustedLength = 0;
			auto lineStart = tempIt;
			while (tempIt != buffer.text.end() && *tempIt != '\n') {
				if (*tempIt == '\t') {
					int nextTabStop = tabWidth - (tabAdjustedLength % tabWidth);
					tabAdjustedLength += nextTabStop;
					nextLineLength += 1;
				}
				else {
					++tabAdjustedLength;
					++nextLineLength;
				}
				++tempIt;
			}
			//更新iter到下一行cursorX的位置，如果光标在制表符中间的位置，则移动到尾部
			int adjustedCursorX = 0;
			buffer.iter = lineStart;
			for (int x = 0; x < nextLineLength; x++) {
				if (adjustedCursorX >= cursorX) {
					break;
				}
				if (*buffer.iter == '\t') {
					int nextTabStop = tabWidth - (adjustedCursorX % tabWidth);
					if (adjustedCursorX + nextTabStop > cursorX) {
						adjustedCursorX += nextTabStop;
						++buffer.iter;
						break;
					}
					else {
						adjustedCursorX += nextTabStop;
					}
				}
				else {
					adjustedCursorX++;
				}
				++buffer.iter;
			}
			//如果下一行长度小于当前cursorX，移动光标到下一行末尾
			if (tabAdjustedLength < cursorX) {
				cursorX = tabAdjustedLength;
			}
			else {
				cursorX = adjustedCursorX;
			}
		}
	}
}

void Editor::onRightKeyPressed(int consoleWidth) {
	if (cursorX < consoleWidth - 1 && buffer.iter != buffer.text.end()) {
		if (buffer.iter != buffer.text.end() && *(buffer.iter) != '\n') {
			if (*buffer.iter == '\t') {
				cursorX += tabWidth - (cursorX % tabWidth);
			}
			else {
				cursorX++;
			}
			buffer.iter++;
		}
	}
}

void Editor::onLeftKeyPressed() {
	if (cursorX > 0 && buffer.iter != buffer.text.begin()) {
		buffer.iter--;
		if (*buffer.iter == '\t') {
			cursorX -= (cursorX % tabWidth) ? (cursorX % tabWidth) : tabWidth;
		}
		else {
			cursorX--;
		}
		cursorX = (std::max)(0, cursorX);
	}
}