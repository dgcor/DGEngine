#pragma once

#include <string>
#include <vector>

namespace FileUtils
{
	bool createDir(const char* dirName) noexcept;

	bool deleteAll(const char* filePath);

	bool deleteFile(const char* filePath) noexcept;

	bool exists(const char* filePath) noexcept;

	std::vector<std::string> getFileList(const std::string& filePath, const std::string& fileExt = "");

	std::string getFileFromPath(const std::string& path);

	std::string getFileWithoutExt(const std::string& fileName);

	std::vector<std::string> getSaveDirList();

	std::string readText(const char* fileName);

	std::vector<uint8_t> readChar(const char* fileName);
	std::vector<uint8_t> readChar(const char* fileName, size_t maxNumBytes);

	const char* getSaveDir() noexcept;
	bool setSaveDir(const char* dirName) noexcept;

	bool saveText(const char* filePath, const char* str, size_t strLen) noexcept;
	bool saveText(const char* filePath, const std::string& str) noexcept;
}
