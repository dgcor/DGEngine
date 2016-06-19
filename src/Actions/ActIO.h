#pragma once

#include "Action.h"
#include "FileUtils.h"
#include "Game.h"

class ActDirCreate : public Action
{
private:
	std::string dir;

public:
	ActDirCreate(const std::string& dir_) : dir(dir_) {}

	virtual bool execute(Game& game)
	{
		FileUtils::createDir(game.getVariableString(dir).c_str());
		return true;
	}
};

class ActFileCopy : public Action
{
private:
	std::string dir;
	std::vector<std::string> filesRead;
	std::string fileWrite;

public:
	ActFileCopy(const std::string& dir_, const std::string& fileRead_,
		const std::string& fileWrite_) : dir(dir_), filesRead(), fileWrite(fileWrite_)
	{
		filesRead.push_back(fileRead_);
	}

	ActFileCopy(const std::string& dir_, const std::vector<std::string>& filesRead_,
		const std::string& fileWrite_) : dir(dir_), filesRead(filesRead_), fileWrite(fileWrite_) {}

	virtual bool execute(Game& game)
	{
		if (filesRead.size() > 0)
		{
			const auto& fileRead = filesRead[0];
			auto str = FileUtils::readText(game.getVariableString(fileRead).c_str());

			for (size_t i = 1; i < filesRead.size(); i++)
			{
				auto param = game.getVariableString(filesRead[i]);
				Utils::replaceStringInPlace(str, "{" + std::to_string(i) + "}", param);
			}
			for (const auto& var : game.getVariables())
			{
				Utils::replaceStringInPlace(str, "%" + var.first + "%", VarUtils::toString(var.second));
			}

			auto writePath = game.getVariableString(dir);
			if (writePath.size() > 0 && Utils::endsWith(writePath, "/") == false)
			{
				writePath += '/';
				FileUtils::createDir(writePath.c_str());
			}
			if (fileWrite.size() > 0)
			{
				writePath += game.getVariableString(fileWrite);
			}
			else
			{
				writePath += game.getVariableString(FileUtils::getFileFromPath(fileRead));
			}
			FileUtils::saveText(writePath.c_str(), str);
		}
		return true;
	}
};

class ActIODelete : public Action
{
private:
	std::string dir;

public:
	ActIODelete(const std::string& dir_) : dir(dir_) {}

	virtual bool execute(Game& game)
	{
		FileUtils::deleteFile(game.getVariableString(dir).c_str());
		return true;
	}
};

class ActIODeleteAll : public Action
{
private:
	std::string dir;

public:
	ActIODeleteAll(const std::string& dir_) : dir(dir_) {}

	virtual bool execute(Game& game)
	{
		FileUtils::deleteAll(game.getVariableString(dir).c_str());
		return true;
	}
};
