#pragma once

#include "Buffer.h"
#include <fstream>
#include <string>

class FileHandler {
public:
	static void ReadFileIntoBuffer(const std::string& filename, Buffer& buffer);
	static void WriteFileFromBuffer(const std::string& filename, const Buffer& buffer);
};