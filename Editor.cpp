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
			char ch = _getch(); //��ȡ����
			//if (ch == 0 || ch == 224) { //���ⰴ��������
			//	ch = _getch();//��ȡ��������
			handleControlKey(ch);//���������backspace
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
		//�ҵ���ǰ�еĿ�ʼ
		while (lineStart != buffer.text.begin() && *prev(lineStart) != '\n') {
			--lineStart;
		}
		//�����ǰ�в����ļ���һ�У������ҵ�һ��
		if (lineStart != buffer.text.begin()) {
			cursorY--;
			--lineStart;//��һ�л��з�
			lineEnd = lineStart;
			//�ҵ���һ�п�ʼ
			while (lineStart != buffer.text.begin() && *prev(lineStart) != '\n') {
				--lineStart;
			}
		}

		//���㵱ǰ�г���
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
		//����iter����һ�е�cursorXλ�ã������ǰλ�����Ʊ���м䣬�ƶ����Ʊ��ĩβ
		int adjustedCursorX = 0;
		buffer.iter = lineStart;
		for (int x = 0; x < charNum; x++) {
			if (adjustedCursorX >= cursorX) { //�������Ѿ��ƶ��򳬹���Ԥ��λ�ã���˵����cursorX��λ��������Ҫ�ƶ����Ʊ��λ�ã�
				break;
			}
			if (*buffer.iter == '\t') {
				int nextTabStop = tabWidth - (adjustedCursorX % tabWidth);
				if (adjustedCursorX + nextTabStop > cursorX) {
					adjustedCursorX += nextTabStop;
					++buffer.iter;//���������⣡
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
		//�����һ�г���С�ڵ�ǰcursorX���ƶ���굽��һ��ĩβ
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
		//�ҵ���ǰ�е���β
		while (tempIt != buffer.text.end() && *tempIt != '\n') {
			++tempIt;
		}
		//�����β�����ļ����һ���ַ����ƶ�����һ��
		if (tempIt != buffer.text.end()) {
			++tempIt;//�������з�
			cursorY++;
			//������һ�г���
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
			//����iter����һ��cursorX��λ�ã����������Ʊ���м��λ�ã����ƶ���β��
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
			//�����һ�г���С�ڵ�ǰcursorX���ƶ���굽��һ��ĩβ
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