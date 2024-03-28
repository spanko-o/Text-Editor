#pragma once

#include <list>
#include <string>

class Buffer {
private:


public:
	std::list<char>::iterator iter;
	std::list<char> text;

public:
	Buffer() :iter(text.end()) {}
	void insertText(const std::string& str);
	void deleteChar();
	std::string getText() const;
	void clear();
	int size();
};