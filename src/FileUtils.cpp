#include "FileUtils.h"
#include <cstring>
#include "PhysFSStream.h"
#include <memory>
#include "Utils.h"

namespace FileUtils
{
	bool createDir(const char* dirName)
	{
		return PHYSFS_mkdir(dirName) != 0;
	}

	bool deleteAll(const char* filePath)
	{
		bool ret = false;
		if (PHYSFS_isDirectory(filePath) != 0)
		{
			auto paths = PHYSFS_enumerateFiles(filePath);
			if (paths != NULL)
			{
				auto writeDir = PHYSFS_getWriteDir();
				if (writeDir != NULL)
				{
					for (char** path = paths; *path != NULL; path++)
					{
						auto fullPath = std::string(filePath) + '/' + *path;

						if (PHYSFS_isDirectory(fullPath.c_str()) != 0)
						{
							deleteAll(fullPath.c_str());
						}
						else
						{
							auto realDir = PHYSFS_getRealDir(fullPath.c_str());
							if (realDir != NULL)
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
		}
		ret = PHYSFS_delete(filePath) != 0;
		return ret;
	}

	bool deleteFile(const char* filePath)
	{
		auto writeDir = PHYSFS_getWriteDir();
		auto realDir = PHYSFS_getRealDir(filePath);
		if (writeDir != NULL && realDir != NULL)
		{
			if (strcmp(writeDir, realDir) == 0)
			{
				return PHYSFS_delete(filePath) != 0;
			}
		}
		return false;
	}

	bool exists(const char* filePath)
	{
		return PHYSFS_exists(filePath) != 0;
	}

	std::vector<std::string> getFileList(const std::string& filePath, const std::string& fileExt)
	{
		std::vector<std::string> vec;
		auto files = PHYSFS_enumerateFiles(filePath.c_str());
		if (files != NULL)
		{
			for (char** file = files; *file != NULL; file++)
			{
				auto file2 = filePath + '/' + std::string(*file);

				if (Utils::endsWith(file2, fileExt) == true &&
					PHYSFS_isDirectory(file2.c_str()) == 0)
				{
					vec.push_back(file2);
				}
			}

			PHYSFS_freeList(files);
		}
		return vec;
	}

	struct MatchPathSeparator
	{
		bool operator()(char ch) const
		{
			return ch == '\\' || ch == '/';
		}
	};

	std::string getFileFromPath(const std::string& path)
	{
		return std::string(std::find_if(path.rbegin(), path.rend(), MatchPathSeparator()).base(), path.end());
	}

	std::string getFileWithoutExt(const std::string& fileName)
	{
		auto pos = fileName.rfind(".");
		if (pos == 0 || pos == std::string::npos)
		{
			return fileName;
		}
		return fileName.substr(0, pos);
	}

	std::vector<std::string> getSaveDirList()
	{
		std::vector<std::string> vecDirs;
		auto dirs = PHYSFS_enumerateFiles("");
		if (dirs != NULL)
		{
			auto writeDir = PHYSFS_getWriteDir();
			if (writeDir != NULL)
			{
				for (char** dir = dirs; *dir != NULL; dir++)
				{
					if (PHYSFS_isDirectory(*dir) != 0)
					{
						auto realDir = PHYSFS_getRealDir(*dir);
						if (realDir != NULL)
						{
							if (strcmp(writeDir, realDir) == 0)
							{
								vecDirs.push_back(*dir);
							}
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
			return "";
		}
		auto data = std::make_unique<char[]>((size_t)ifs.getSize());
		ifs.read(data.get(), ifs.getSize());

		return std::string(data.get(), (unsigned)ifs.getSize());
	}

	std::vector<uint8_t> readChar(const char* fileName)
	{
		sf::PhysFSStream ifs(fileName);
		if (ifs.hasError() == true)
		{
			return std::vector<uint8_t>();
		}
		std::vector<uint8_t> data((size_t)ifs.getSize());
		ifs.read(data.data(), ifs.getSize());

		return data;
	}

	const char* getSaveDir()
	{
		return PHYSFS_getWriteDir();
	}

	bool setSaveDir(const char* dirName)
	{
		auto userDir = PHYSFS_getUserDir();
		if (PHYSFS_setWriteDir(userDir) != 0)
		{
			if (PHYSFS_mkdir(dirName) != 0)
			{
				auto writeDir = std::string(userDir) + dirName;
				return PHYSFS_setWriteDir(writeDir.c_str()) != 0;
			}
		}
		return false;
	}

	bool saveText(const char* filePath, const char* str, size_t strLen)
	{
		auto file = PHYSFS_openWrite(filePath);
		if (file != NULL)
		{
			PHYSFS_write(file, str, 1, strLen);
			return PHYSFS_close(file) != 0;
		}
		return false;
	}

	bool saveText(const char* filePath, const std::string& str)
	{
		return saveText(filePath, str.c_str(), str.size());
	}
}
