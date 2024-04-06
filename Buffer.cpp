#include "Buffer.h"


//在迭代器位置插入文本
void Buffer::insertText(const std::string& str) {
	if (str.empty()) return;

	iter = (text.empty()) ? text.begin() : iter;

	for (auto ch : str) {
		iter = text.insert(iter, ch);
		++iter;
	}
}

//删除指定位置字符
void Buffer::deleteChar() {
	if (iter != text.begin()) {
		auto toErase = iter;
		--toErase;
		text.erase(toErase);
	}
}

//获取缓冲区文本内容
std::string Buffer::getText() const {
	std::string str(text.begin(), text.end());
	return str;
}

//获取缓冲群当前行内容
std::string Buffer::getCurrentLine(std::list<char>::iterator it) {
	auto lineStart= it;
	auto lineEnd = it;

	while (lineStart != text.begin() && *prev(lineStart) != '\n') {
		lineStart--;
	}
	lineEnd = std::find(it, text.end(), '\n');
	return std::string(lineStart, lineEnd);
}

//清空缓冲区
void Buffer::clear() {
	text.clear();
}

//计算缓冲区大小
int Buffer::size() {
	return text.size();
}