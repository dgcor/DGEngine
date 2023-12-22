#include "FileUtils.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include "Hooks.h"
#include <memory>
#include "SFML/PhysFSStream.h"
#include "Utils/Log.h"
#include "Utils/Utils.h"

namespace FileUtils
{
	static const char* mainArgv0{ nullptr };

	void initPhysFS(const char* argv0)
	{
		mainArgv0 = argv0;
		initPhysFS();
	}

	void initPhysFS()
	{
		deinitPhysFS();
		SPDLOG_DEBUG("initPhysFS");
		if (PHYSFS_init(mainArgv0) != 0)
		{
			if (Hooks::RegisterArchivers != nullptr)
			{
				SPDLOG_DEBUG("RegisterArchivers");
				Hooks::RegisterArchivers();
			}
			PHYSFS_permitSymbolicLinks(1);
		}
	}

	void deinitPhysFS()
	{
		if (PHYSFS_isInit() != 0)
		{
			SPDLOG_DEBUG("deinitPhysFS");
			PHYSFS_deinit();
		}
	}

	bool gamefileExists(const std::string_view filePath)
	{
		try
		{
			std::u8string_view utf8FilePath((const char8_t*)filePath.data(), filePath.size());
			std::filesystem::path path(utf8FilePath);

			if (std::filesystem::exists(path) == true)
			{
				return true;
			}
			if (path.has_extension() == true)
			{
				path = path.replace_extension();
				if (std::filesystem::exists(path) == true)
				{
					return true;
				}
			}
			for (const auto& ext : Hooks::ArchiveExtensions)
			{
				path = path.replace_extension(ext);
				if (std::filesystem::exists(path) == true)
				{
					return true;
				}
			}
		}
		catch (std::exception&)
		{
			SPDLOG_DEBUG("gamefileExists error: {}", filePath);
		}
		SPDLOG_DEBUG("gamefile doesn't exist: {}", filePath);
		return false;
	}

	std::string makePhysfsPath(const std::string_view path, const std::string_view file)
	{
		std::string filePath(path);
		if (filePath.empty() == false &&
			filePath.ends_with('/') == false)
		{
			filePath += '/';
		}
		filePath += file;
		return filePath;
	}

	std::string makeSystemPath(const std::string_view path, const std::string_view file)
	{
		std::string filePath(path);
		if (filePath.empty() == false &&
			filePath.ends_with('\\') == false &&
			filePath.ends_with('/') == false)
		{
			filePath += PHYSFS_getDirSeparator();
		}
		filePath += file;
		return filePath;
	}

	bool mount(const std::string_view file, const std::string_view mountPoint,
		bool appendToSearchPath)
	{
		int append = appendToSearchPath == true ? 1 : 0;
		try
		{
			std::u8string_view utf8File((const char8_t*)file.data(), file.size());
			std::filesystem::path path(utf8File);
			auto utf8Path = path.u8string();

			if (PHYSFS_mount((const char*)utf8Path.c_str(), mountPoint.data(), append) != 0)
			{
				SPDLOG_INFO(R"(mount: "{}" -> "{}")", (const char*)utf8Path.c_str(), mountPoint);
				return true;
			}
			if (path.has_extension() == true)
			{
				path = path.replace_extension();
				utf8Path = path.u8string();
				if (PHYSFS_mount((const char*)utf8Path.c_str(), mountPoint.data(), append) != 0)
				{
					SPDLOG_INFO(R"(mount: "{}" -> "{}")", (const char*)utf8Path.c_str(), mountPoint);
					return true;
				}
			}
			for (const auto& ext : Hooks::ArchiveExtensions)
			{
				path = path.replace_extension(ext);
				utf8Path = path.u8string();
				if (PHYSFS_mount((const char*)utf8Path.c_str(), mountPoint.data(), append) != 0)
				{
					SPDLOG_INFO(R"(mount: "{}" -> "{}")", (const char*)utf8Path.c_str(), mountPoint);
					return true;
				}
			}
		}
		catch (std::exception&)
		{
			SPDLOG_DEBUG(R"(mount error: "{}" -> "{}")", file, mountPoint);
		}
		SPDLOG_ERROR(R"(mount: "{}" -> "{}")", file, mountPoint);
		return false;
	}

	bool unmount(const std::string_view file)
	{
		if (PHYSFS_unmount(file.data()) != 0)
		{
			SPDLOG_INFO("unmount: {}", file);
			return true;
		}
		try
		{
			std::u8string_view utf8File((const char8_t*)file.data(), file.size());
			std::filesystem::path path(utf8File);
			std::u8string utf8Path;

			if (path.has_extension() == true)
			{
				path = path.replace_extension();
				utf8Path = path.u8string();
				if (PHYSFS_unmount((const char*)utf8Path.c_str()) != 0)
				{
					SPDLOG_INFO("unmount: {}", (const char*)utf8Path.c_str());
					return true;
				}
			}
			for (const auto& ext : Hooks::ArchiveExtensions)
			{
				path = path.replace_extension(ext);
				utf8Path = path.u8string();
				if (PHYSFS_unmount((const char*)utf8Path.c_str()) != 0)
				{
					SPDLOG_INFO("unmount: {}", (const char*)utf8Path.c_str());
					return true;
				}
			}
		}
		catch (std::exception&)
		{
			SPDLOG_DEBUG("unmount error: {}", file);
		}
		SPDLOG_ERROR("unmount: {}", file);
		return false;
	}

	void unmountAll()
	{
		SPDLOG_DEBUG("unmountAll");
		initPhysFS(nullptr);
	}

	bool copyDir(const char* dirSrcName, const char* dirDstName)
	{
		PHYSFS_Stat stat;
		if (PHYSFS_stat(dirSrcName, &stat) == 0 ||
			stat.filetype != PHYSFS_FILETYPE_DIRECTORY)
		{
			return false;
		}
		if (PHYSFS_stat(dirDstName, &stat) != 0 &&
			stat.filetype != PHYSFS_FILETYPE_DIRECTORY)
		{
			return false;
		}

		createDir(dirDstName);
		auto paths = PHYSFS_enumerateFiles(dirSrcName);
		if (paths != nullptr)
		{
			for (char** path = paths; *path != nullptr; path++)
			{
				auto fullSrcPath = makePhysfsPath(dirSrcName, *path);
				auto fullDstPath = makePhysfsPath(dirDstName, *path);

				if (PHYSFS_stat(fullSrcPath.c_str(), &stat) == 0)
				{
					continue;
				}
				if (stat.filetype == PHYSFS_FILETYPE_DIRECTORY)
				{
					copyDir(fullSrcPath.c_str(), fullDstPath.c_str());
				}
				else
				{
					// copy file (read source file and write destination)
					sf::PhysFSStream fileRead(fullSrcPath);
					auto fileWrite = PHYSFS_openWrite(fullDstPath.c_str());
					if (fileRead.hasError() == false &&
						fileWrite != nullptr)
					{
						FileBytes data((size_t)fileRead.getSize());
						fileRead.read(data.data(), fileRead.getSize());
						PHYSFS_writeBytes(fileWrite, data.data(), data.size());
						PHYSFS_close(fileWrite);
					}
				}
			}
			PHYSFS_freeList(paths);
			return true;
		}
		return false;
	}

	bool createDir(const char* dirName) noexcept
	{
		auto success = PHYSFS_mkdir(dirName) != 0;
		SPDLOG_DEBUG("createDir ({}): {}", success, dirName);
		return success;
	}

	bool deleteAll(const char* filePath, bool deleteRoot)
	{
		PHYSFS_Stat fileStat;
		if (PHYSFS_stat(filePath, &fileStat) == 0)
		{
			return false;
		}
		bool ret = false;
		if (fileStat.filetype == PHYSFS_FILETYPE_DIRECTORY)
		{
			auto paths = PHYSFS_enumerateFiles(filePath);
			if (paths != nullptr)
			{
				auto writeDir = PHYSFS_getWriteDir();
				if (writeDir != nullptr)
				{
					for (char** path = paths; *path != nullptr; path++)
					{
						auto fullPath = makePhysfsPath(filePath, *path);
						if (PHYSFS_stat(fullPath.c_str(), &fileStat) == 0)
						{
							continue;
						}
						if (fileStat.filetype == PHYSFS_FILETYPE_DIRECTORY)
						{
							deleteAll(fullPath.c_str(), true);
						}
						else
						{
							auto realDir = PHYSFS_getRealDir(fullPath.c_str());
							if (realDir != nullptr)
							{
								if (std::strcmp(writeDir, realDir) == 0)
								{
									ret = PHYSFS_delete(fullPath.c_str()) != 0;
								}
							}
						}
					}
				}
				PHYSFS_freeList(paths);
			}
			if (deleteRoot == true)
			{
				ret = PHYSFS_delete(filePath) != 0;
			}
		}
		else
		{
			ret = PHYSFS_delete(filePath) != 0;
		}
		return ret;
	}

	bool deleteFile(const char* filePath) noexcept
	{
		auto writeDir = PHYSFS_getWriteDir();
		auto realDir = PHYSFS_getRealDir(filePath);
		if (writeDir != nullptr && realDir != nullptr)
		{
			if (strcmp(writeDir, realDir) == 0)
			{
				return PHYSFS_delete(filePath) != 0;
			}
		}
		return false;
	}

	bool exists(const char* filePath) noexcept
	{
		auto fileExists = PHYSFS_exists(filePath) != 0;
#ifdef DGENGINE_FALLBACK_TO_LOWERCASE
		if (fileExists == false)
		{
			auto lowerCasefilePath = Utils::toLower(filePath);
			fileExists = PHYSFS_exists(lowerCasefilePath.c_str()) != 0;
		}
#endif
		SPDLOG_DEBUG("fileExists ({}): {}", fileExists, filePath);
		return fileExists;
	}

	std::vector<std::string> geDirList(const std::string_view path, bool onlyWritableDirs)
	{
		std::string_view saveDir;
		if (onlyWritableDirs == true)
		{
			auto writeDir = PHYSFS_getWriteDir();
			if (writeDir != nullptr)
			{
				saveDir = writeDir;
			}
		}
		std::vector<std::string> vecDirs;
		auto dirs = PHYSFS_enumerateFiles(path.data());
		if (dirs != nullptr)
		{
			PHYSFS_Stat fileStat;
			for (char** dir = dirs; *dir != nullptr; dir++)
			{
				auto dirPath = makePhysfsPath(path, *dir);
				if (PHYSFS_stat(dirPath.c_str(), &fileStat) == 0)
				{
					continue;
				}
				if (fileStat.filetype != PHYSFS_FILETYPE_DIRECTORY)
				{
					continue;
				}
				if (**dir == '.')
				{
					continue;
				}
				if (onlyWritableDirs == true)
				{
					auto realDir = PHYSFS_getRealDir(*dir);
					if (realDir != nullptr &&
						saveDir != realDir)
					{
						continue;
					}
				}
				vecDirs.push_back(*dir);
			}
			PHYSFS_freeList(dirs);
		}
		return vecDirs;
	}

	std::vector<std::string> getFileList(const std::string_view path,
		const std::string_view fileExt, bool getFullPath)
	{
		std::vector<std::string> vec;
		auto files = PHYSFS_enumerateFiles(path.data());
		if (files != nullptr)
		{
			PHYSFS_Stat fileStat;
			for (char** file = files; *file != nullptr; file++)
			{
				auto fullPath = makePhysfsPath(path, *file);

				if (fullPath.ends_with(fileExt) == false)
				{
					continue;
				}
				if (PHYSFS_stat(fullPath.c_str(), &fileStat) == 0)
				{
					continue;
				}
				if (fileStat.filetype == PHYSFS_FILETYPE_REGULAR)
				{
					if (getFullPath == true)
					{
						vec.push_back(fullPath);
					}
					else
					{
						vec.push_back(std::string(*file));
					}
				}
			}
			PHYSFS_freeList(files);
		}
		return vec;
	}

	std::string getFileName(const std::string_view filePath)
	{
		try
		{
			std::u8string_view utf8FilePath((const char8_t*)filePath.data(), filePath.size());
			auto utf8Str = std::filesystem::path(utf8FilePath).filename().u8string();
			return std::string((const char*)utf8Str.data(), utf8Str.size());
		}
		catch (std::exception&) {}
		return {};
	}

	std::string getFileNameWithoutExt(const std::string_view filePath)
	{
		try
		{
			std::u8string_view utf8FilePath((const char8_t*)filePath.data(), filePath.size());
			auto utf8Str = std::filesystem::path(utf8FilePath).stem().u8string();
			return std::string((const char*)utf8Str.data(), utf8Str.size());
		}
		catch (std::exception&) {}
		return {};
	}

	std::string getFileExtension(const std::string_view filePath)
	{
		try
		{
			std::u8string_view utf8FilePath((const char8_t*)filePath.data(), filePath.size());
			auto utf8Str = std::filesystem::path(utf8FilePath).extension().u8string();
			return std::string((const char*)utf8Str.data(), utf8Str.size());
		}
		catch (std::exception&) {}
		return {};
	}

	std::string getFilePath(const std::string_view filePath)
	{
		try
		{
			std::u8string_view utf8FilePath((const char8_t*)filePath.data(), filePath.size());
			auto utf8Str = std::filesystem::path(utf8FilePath).parent_path().u8string();
			return std::string((const char*)utf8Str.data(), utf8Str.size());
		}
		catch (std::exception&) {}
		return {};
	}

	FileBytes readBytes(const std::string_view fileName)
	{
		return readBytes(fileName, std::numeric_limits<size_t>::max());
	}

	FileBytes readBytes(const std::string_view fileName, size_t maxNumBytes)
	{
		return readBytes(fileName, 0, maxNumBytes);
	}

	FileBytes readBytes(const std::string_view fileName, size_t startPosition, size_t maxNumBytes)
	{
		sf::PhysFSStream ifs(fileName);
		if (ifs.hasError() == true)
		{
			SPDLOG_DEBUG("readBytes error: {}", fileName);
			return {};
		}
		auto fileSize = (size_t)ifs.getSize();

		startPosition = std::min(startPosition, fileSize);
		maxNumBytes = std::min(maxNumBytes, fileSize);
		size_t endPosition = std::min(startPosition + maxNumBytes, fileSize);

		auto size = endPosition - startPosition;
		if (size == 0)
		{
			SPDLOG_DEBUG("readBytes empty range: {}", fileName);
			return {};
		}

		FileBytes data(size);
		if (startPosition > 0)
		{
			ifs.seek(startPosition);
		}
		ifs.read(data.data(), size);
		return data;
	}

	std::string readText(const std::string_view fileName)
	{
		sf::PhysFSStream ifs(fileName);
		if (ifs.hasError() == true)
		{
			SPDLOG_DEBUG("readText error: {}", fileName);
			return {};
		}
		std::string data((size_t)ifs.getSize(), '\0');
		ifs.read(data.data(), ifs.getSize());
		return data;
	}

	std::string getSaveDir()
	{
		auto writeDir = PHYSFS_getWriteDir();
		if (writeDir != nullptr)
		{
			return makeSystemPath(writeDir, {});
		}
		return {};
	}

	bool setSaveDir(const char* dirName) noexcept
	{
		std::u8string userDirStr;
		const char* userDir = nullptr;
		try
		{
			auto portablePath = std::filesystem::current_path() / u8"portable";
			if (std::filesystem::exists(portablePath) == true &&
				std::filesystem::is_directory(portablePath) == true)
			{
				portablePath /= (const char8_t*)dirName;
				userDirStr = portablePath.u8string();
				userDir = (const char*)userDirStr.c_str();
				std::filesystem::create_directories(portablePath);
			}
		}
		catch (std::exception&) {}

		if (userDir == nullptr)
		{
			userDir = PHYSFS_getPrefDir("DGEngine", dirName);
		}
		if (userDir == nullptr)
		{
			return false;
		}
		return PHYSFS_setWriteDir(userDir) != 0;
	}

	bool saveBytes(const std::string_view filePath, const std::span<std::byte> bytes) noexcept
	{
		try
		{
			std::u8string_view utf8FilePath((const char8_t*)filePath.data(), filePath.size());
			std::filesystem::path path(utf8FilePath);
			if (path.has_parent_path() == true)
			{
				createDir((const char*)path.parent_path().u8string().c_str());
			}
			auto file = PHYSFS_openWrite(filePath.data());
			if (file != nullptr)
			{
				PHYSFS_writeBytes(file, bytes.data(), bytes.size());
				return PHYSFS_close(file) != 0;
			}
		}
		catch (std::exception&)
		{
			SPDLOG_DEBUG("saveBytes error: {}", filePath);
		}
		SPDLOG_WARN("saveBytes failed: {}", filePath);
		return false;
	}

	bool saveText(const std::string_view filePath, const std::string_view str) noexcept
	{
		return saveBytes(filePath, std::span<std::byte>((std::byte*)str.data(), str.size()));
	}

	bool exportFile(const std::string_view inFile, const std::string_view outFile)
	{
		try
		{
			std::u8string_view utf8InFile((const char8_t*)inFile.data(), inFile.size());
			std::filesystem::path inPath(utf8InFile);
			std::u8string_view utf8OutFile((const char8_t*)outFile.data(), outFile.size());
			std::filesystem::path outPath(utf8OutFile);

			if (std::filesystem::is_directory(outPath) == true &&
				inPath.has_filename() == true)
			{
				outPath /= inPath.filename();
			}

			auto data = readBytes(inFile);
			auto newFile = std::fstream(outPath, std::ios::out | std::ios::binary);
			newFile.write((const char*)data.data(), data.size());
			newFile.close();
			return true;
		}
		catch (std::exception&) {}
		SPDLOG_WARN(R"(exportFile failed: "{}" -> "{}")", inFile, outFile);
		return false;
	}
}
