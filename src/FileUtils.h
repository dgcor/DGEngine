#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace FileUtils
{
	void initPhysFS(const char* argv0);
	void deinitPhysFS();

	bool gamefileExists(const std::string_view filePath);

	// add folder or archive to the search path.
	bool mount(const std::string_view file, const std::string_view mountPoint,
		bool appendToSearchPath);

	// remove folder or archive from the search path.
	// only call after all file handles for the search path are closed.
	bool unmount(const std::string_view file);

	// removes all folders and archives from the search path.
	// only call after all file handles are closed.
	void unmountAll();

	bool copyDir(const char* dirSrcName, const char* dirDstName);

	bool createDir(const char* dirName) noexcept;

	// deletes all files in folder. if deleteRoot is false,
	// the directory "filePath" is empty, but isn't deleted.
	bool deleteAll(const char* filePath, bool deleteRoot = true);

	bool deleteFile(const char* filePath) noexcept;

	bool exists(const char* filePath) noexcept;

	std::vector<std::string> getFileList(const std::string_view filePath,
		const std::string_view fileExt, bool getFullPath);

	std::string getFileName(const std::string_view filePath);

	std::string getFileNameWithoutExt(const std::string_view filePath);

	std::string getFileExtension(const std::string_view filePath);

	std::string getFilePath(const std::string_view filePath);

	std::vector<std::string> geDirList(const std::string_view path,
		const std::string_view rootPath);

	std::vector<std::string> getSaveDirList();

	std::string readText(const std::string_view fileName);

	std::vector<uint8_t> readChar(const std::string_view fileName);
	std::vector<uint8_t> readChar(const std::string_view, size_t maxNumBytes);

	std::string getSaveDir();
	bool setSaveDir(const char* dirName) noexcept;

	// creates path if it doesn't exist
	bool saveText(const std::string_view filePath, const std::string_view str) noexcept;

	// writes file to a filesystem path (not to physfs's write dir path).
	bool exportFile(const std::string_view inFile, const std::string_view outFile);
}
