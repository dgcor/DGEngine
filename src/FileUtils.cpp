#include "FileUtils.h"
#include <cstring>
#include "PhysFSStream.h"
#include <memory>
#include "Utils/Utils.h"

namespace FileUtils
{
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
		return PHYSFS_exists(filePath) != 0;
	}

	std::vector<std::string> getFileList(const std::string& filePath, const std::string_view fileExt)
	{
		std::vector<std::string> vec;
		auto files = PHYSFS_enumerateFiles(filePath.c_str());
		if (files != nullptr)
		{
			PHYSFS_Stat fileStat;
			for (char** file = files; *file != nullptr; file++)
			{
				auto file2 = filePath + '/' + std::string(*file);

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
					vec.push_back(file2);
				}
			}
			PHYSFS_freeList(files);
		}
		return vec;
	}

	std::string getFileFromPath(const std::string_view path)
	{
		auto found = path.find_last_of("/\\");
		if (found != std::string::npos)
		{
			return std::string(path.substr(found + 1));
		}
		return std::string(path);
	}

	std::string getFileWithoutExt(const std::string_view fileName)
	{
		auto pos = fileName.rfind(".");
		if (pos == 0 || pos == std::string::npos)
		{
			return std::string(fileName);
		}
		return std::string(fileName.substr(0, pos));
	}

	std::string getPathFromFile(const std::string_view path)
	{
		auto found = path.find_last_of("/\\");
		if (found != std::string::npos)
		{
			return std::string(path.substr(0, found));
		}
		return {};
	}

	std::vector<std::string> getSaveDirList()
	{
		std::vector<std::string> vecDirs;
		auto dirs = PHYSFS_enumerateFiles("");
		if (dirs != nullptr)
		{
			auto writeDir = PHYSFS_getWriteDir();
			if (writeDir != nullptr)
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
					auto realDir = PHYSFS_getRealDir(*dir);
					if (realDir != nullptr)
					{
						if (strcmp(writeDir, realDir) == 0)
						{
							vecDirs.push_back(*dir);
						}
					}
				}
			}
			PHYSFS_freeList(dirs);
		}
		return vecDirs;
	}

	std::string readText(const char* fileName)
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

	std::vector<uint8_t> readChar(const char* fileName)
	{
		return readChar(fileName, std::numeric_limits<size_t>::max());
	}

	std::vector<uint8_t> readChar(const char* fileName, size_t maxNumBytes)
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

	const char* getSaveDir() noexcept
	{
		return PHYSFS_getWriteDir();
	}

	bool setSaveDir(const char* dirName) noexcept
	{
#ifdef __ANDROID__
		auto userDir = "data/data/com.dgengine/files/";
#else
		auto userDir = PHYSFS_getPrefDir("DGEngine", dirName);
#endif
		if (userDir == nullptr)
		{
			return false;
		}
		return PHYSFS_setWriteDir(userDir) != 0;
	}

	bool saveText(const std::string_view filePath, const std::string_view str) noexcept
	{
		auto path = getPathFromFile(filePath);
		if (path.empty() == false)
		{
			createDir(path.c_str());
		}
		auto file = PHYSFS_openWrite(filePath.data());
		if (file != nullptr)
		{
			PHYSFS_writeBytes(file, str.data(), str.size());
			return PHYSFS_close(file) != 0;
		}
		return false;
	}
}
