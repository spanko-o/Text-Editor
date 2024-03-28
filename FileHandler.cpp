#include "FileHandler.h"

void FileHandler::ReadFileIntoBuffer(const std::string& filename, Buffer& buffer) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file for reading.");
	}

	buffer.clear();
	char ch;
	while (file.get(ch)) {
		buffer.insertText(std::string(1, ch));
	}

	file.close();
}

void FileHandler::WriteFileFromBuffer(const std::string& filename, const Buffer& buffer) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file for writing.");
	}

	std::string text = buffer.getText();
	file << text;

	file.close();
}