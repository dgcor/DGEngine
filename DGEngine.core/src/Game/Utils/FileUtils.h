#pragma once

#include <Resources/FileBytes.h>
#include <span>
#include <string>
#include <string_view>

namespace FileUtils
{
	void initPhysFS(const char* argv0);
	void initPhysFS();
	void deinitPhysFS();

	bool gamefileExists(const std::string_view filePath);

	// concatenates physfs path and file/folder
	std::string makePhysfsPath(const std::string_view path, const std::string_view file);

	// concatenates local OS path and file/folder
	std::string makeSystemPath(const std::string_view path, const std::string_view file);

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
	// if the filePath to delete is in a mount point, it will fail.
	bool deleteAll(const char* filePath, bool deleteRoot = true);

	// if the filePath to delete is in a mount point, it will fail.
	bool deleteFile(const char* filePath) noexcept;

	bool exists(const char* filePath) noexcept;

	// get list of dirs in path.
	std::vector<std::string> geDirList(const std::string_view path, bool onlyWritableDirs);

	// get list of fileNames or fullPaths in path
	std::vector<std::string> getFileList(const std::string_view path,
		const std::string_view fileExt, bool getFullPath);

	std::string getFileName(const std::string_view filePath);

	std::string getFileNameWithoutExt(const std::string_view filePath);

	std::string getFileExtension(const std::string_view filePath);

	std::string getFilePath(const std::string_view filePath);

	FileBytes readBytes(const std::string_view fileName);
	FileBytes readBytes(const std::string_view, size_t maxNumBytes);
	FileBytes readBytes(const std::string_view, size_t startPosition, size_t maxNumBytes);

	std::string readText(const std::string_view fileName);

	std::string getSaveDir();

	bool setSaveDir(const char* dirName) noexcept;

	// creates path if it doesn't exist
	bool saveBytes(const std::string_view filePath, const std::span<std::byte> bytes) noexcept;

	// creates path if it doesn't exist
	bool saveText(const std::string_view filePath, const std::string_view str) noexcept;

	// writes file to a filesystem path (not to physfs's write dir path).
	bool exportFile(const std::string_view inFile, const std::string_view outFile);
}
