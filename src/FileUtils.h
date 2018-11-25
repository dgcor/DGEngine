#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace FileUtils
{
	bool copyDir(const char* dirSrcName, const char* dirDstName);

	bool createDir(const char* dirName) noexcept;

	// deletes all files in folder. if deleteRoot is false,
	// the directory "filePath" is empty, but isn't deleted.
	bool deleteAll(const char* filePath, bool deleteRoot = true);

	bool deleteFile(const char* filePath) noexcept;

	bool exists(const char* filePath) noexcept;

	std::vector<std::string> getFileList(const std::string& filePath, const std::string_view fileExt = "");

	std::string getFileFromPath(const std::string_view path);

	std::string getFileWithoutExt(const std::string_view fileName);

	std::string getPathFromFile(const std::string_view path);

	std::vector<std::string> getSaveDirList();

	std::string readText(const char* fileName);

	std::vector<uint8_t> readChar(const char* fileName);
	std::vector<uint8_t> readChar(const char* fileName, size_t maxNumBytes);

	const char* getSaveDir() noexcept;
	bool setSaveDir(const char* dirName) noexcept;

	// creates path if it doesn't exist
	bool saveText(const std::string_view filePath, const std::string_view str) noexcept;
}
