#include "FileUtils.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include "Hooks.h"
#include <memory>
#include "SFML/PhysFSStream.h"
#include "Utils/Utils.h"

namespace FileUtils
{
	void initPhysFS(const char* argv0)
	{
		static const char* mainArgv0 = argv0;
		deinitPhysFS();
		if (PHYSFS_init(mainArgv0) != 0)
		{
			if (Hooks::RegisterArchivers != nullptr)
			{
				Hooks::RegisterArchivers();
			}
			PHYSFS_permitSymbolicLinks(1);
		}
	}

	void deinitPhysFS()
	{
		if (PHYSFS_isInit() != 0)
		{
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
		catch (std::exception&) {}
		return false;
	}

	bool mount(const std::string_view file, const std::string_view mountPoint,
		bool appendToSearchPath)
	{
		int append = appendToSearchPath == true ? 1 : 0;
		try
		{
			std::u8string_view utf8File((const char8_t*)file.data(), file.size());
			std::filesystem::path path(utf8File);

			if (PHYSFS_mount((const char*)path.u8string().c_str(), mountPoint.data(), append) != 0)
			{
				return true;
			}
			if (path.has_extension() == true)
			{
				path = path.replace_extension();
				if (PHYSFS_mount((const char*)path.u8string().c_str(), mountPoint.data(), append) != 0)
				{
					return true;
				}
			}
			for (const auto& ext : Hooks::ArchiveExtensions)
			{
				path = path.replace_extension(ext);
				if (PHYSFS_mount((const char*)path.u8string().c_str(), mountPoint.data(), append) != 0)
				{
					return true;
				}
			}
		}
		catch (std::exception&) {}
		return false;
	}

	bool unmount(const std::string_view file)
	{
		if (PHYSFS_unmount(file.data()) != 0)
		{
			return true;
		}
		try
		{
			std::u8string_view utf8File((const char8_t*)file.data(), file.size());
			std::filesystem::path path(utf8File);

			if (path.has_extension() == true)
			{
				path = path.replace_extension();
				if (PHYSFS_unmount((const char*)path.u8string().c_str()) != 0)
				{
					return true;
				}
			}
			for (const auto& ext : Hooks::ArchiveExtensions)
			{
				path = path.replace_extension(ext);
				if (PHYSFS_unmount((const char*)path.u8string().c_str()) != 0)
				{
					return true;
				}
			}
		}
		catch (std::exception&) {}
		return false;
	}

	void unmountAll()
	{
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
				auto fullSrcPath = std::string(dirSrcName) + '/' + *path;
				auto fullDstPath = std::string(dirDstName) + '/' + *path;

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
						std::vector<uint8_t> data((size_t)fileRead.getSize());
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
		return PHYSFS_mkdir(dirName) != 0;
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
						auto fullPath = std::string(filePath) + '/' + *path;
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
		return fileExists;
	}

	std::vector<std::string> getFileList(const std::string_view filePath,
		const std::string_view fileExt, bool getFullPath)
	{
		std::vector<std::string> vec;
		auto files = PHYSFS_enumerateFiles(filePath.data());
		if (files != nullptr)
		{
			PHYSFS_Stat fileStat;
			for (char** file = files; *file != nullptr; file++)
			{
				auto file2 = std::string(filePath) + '/' + std::string(*file);

				if (Utils::endsWith(file2, fileExt) == false)
				{
					continue;
				}
				if (PHYSFS_stat(file2.c_str(), &fileStat) == 0)
				{
					continue;
				}
				if (fileStat.filetype == PHYSFS_FILETYPE_REGULAR)
				{
					if (getFullPath == true)
					{
						vec.push_back(file2);
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

	std::vector<std::string> geDirList(const std::string_view path,
		const std::string_view rootPath)
	{
		std::vector<std::string> vecDirs;
		auto dirs = PHYSFS_enumerateFiles(path.data());
		if (dirs != nullptr)
		{
			PHYSFS_Stat fileStat;
			for (char** dir = dirs; *dir != nullptr; dir++)
			{
				if (PHYSFS_stat(*dir, &fileStat) == 0)
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
				if (rootPath.empty() == false)
				{
					auto realDir = PHYSFS_getRealDir(*dir);
					if (realDir != nullptr)
					{
						if (rootPath != realDir)
						{
							continue;
						}
					}
				}
				vecDirs.push_back(*dir);
			}
			PHYSFS_freeList(dirs);
		}
		return vecDirs;
	}

	std::vector<std::string> getSaveDirList()
	{
		auto writeDir = PHYSFS_getWriteDir();
		if (writeDir != nullptr)
		{
			return geDirList("", writeDir);
		}
		return {};
	}

	std::string readText(const std::string_view fileName)
	{
		sf::PhysFSStream ifs(fileName);
		if (ifs.hasError() == true)
		{
			return {};
		}
		std::string data((size_t)ifs.getSize(), '\0');
		ifs.read(data.data(), ifs.getSize());
		return data;
	}

	std::vector<uint8_t> readChar(const std::string_view fileName)
	{
		return readChar(fileName, std::numeric_limits<size_t>::max());
	}

	std::vector<uint8_t> readChar(const std::string_view fileName, size_t maxNumBytes)
	{
		sf::PhysFSStream ifs(fileName);
		if (ifs.hasError() == true)
		{
			return {};
		}
		auto size = std::min((size_t)ifs.getSize(), maxNumBytes);
		std::vector<uint8_t> data(size);
		ifs.read(data.data(), size);
		return data;
	}

	std::string getSaveDir()
	{
		std::string saveDir = PHYSFS_getWriteDir();
		if (Utils::endsWith(saveDir, "\\") == false &&
			Utils::endsWith(saveDir, "/") == false)
		{
			saveDir += PHYSFS_getDirSeparator();
		}
		return saveDir;
	}

	bool setSaveDir(const char* dirName) noexcept
	{
		std::u8string userDirStr;
		const char* userDir = nullptr;
		try
		{
			std::u8string_view utf8BaseDir((const char8_t*)PHYSFS_getBaseDir());
			std::filesystem::path portablePath(utf8BaseDir);
			portablePath /= u8"portable";
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

	bool saveText(const std::string_view filePath, const std::string_view str) noexcept
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
				PHYSFS_writeBytes(file, str.data(), str.size());
				return PHYSFS_close(file) != 0;
			}
		}
		catch (std::exception&) {}
		return false;
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

			auto data = readChar(inFile);
			auto newFile = std::fstream(outPath, std::ios::out | std::ios::binary);
			newFile.write((char*)&data[0], data.size());
			newFile.close();
		}
		catch (std::exception&) {}
		return false;
	}
}
