#include "Buffer.h"


//�ڵ�����λ�ò����ı�
void Buffer::insertText(const std::string& str) {
	if (str.empty()) return;

	iter = (text.empty()) ? text.begin() : iter;

	for (auto ch : str) {
		iter = text.insert(iter, ch);
		++iter;
	}
}

//ɾ��ָ��λ���ַ�
void Buffer::deleteChar() {
	if (iter != text.begin()) {
		auto toErase = iter;
		--toErase;
		text.erase(toErase);
	}
}

//��ȡ�������ı�����
std::string Buffer::getText() const {
	std::string str(text.begin(), text.end());
	return str;
}

//��ȡ����Ⱥ��ǰ������
std::string Buffer::getCurrentLine(std::list<char>::iterator it) {
	auto lineStart= it;
	auto lineEnd = it;

	while (lineStart != text.begin() && *prev(lineStart) != '\n') {
		lineStart--;
	}
	lineEnd = std::find(it, text.end(), '\n');
	return std::string(lineStart, lineEnd);
}

//��ջ�����
void Buffer::clear() {
	text.clear();
}

//���㻺������С
int Buffer::size() {
	return text.size();
}