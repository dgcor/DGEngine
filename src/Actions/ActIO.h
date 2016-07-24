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
	std::string nullText;

public:
	ActFileCopy(const std::string& dir_, const std::vector<std::string>& filesRead_,
		const std::string& fileWrite_, const std::string& nullText_)
		: dir(dir_), filesRead(filesRead_), fileWrite(fileWrite_), nullText(nullText_) {}

	virtual bool execute(Game& game)
	{
		if (filesRead.size() > 0)
		{
			const auto& fileRead = filesRead[0];
			auto str = FileUtils::readText(game.getVariableString(fileRead).c_str());

			std::string param;
			Variable var2;

			for (size_t i = 1; i < filesRead.size(); i++)
			{
				const auto& varStr = filesRead[i];

				if ((varStr.size() > 2) &&
					(varStr.front() == '%') &&
					(varStr.back() == '%'))
				{
					if (game.getVariable(filesRead[i], var2) == true)
					{
						param = VarUtils::toString(var2);
					}
					else
					{
						param = nullText;
					}
				}
				else if ((varStr.size() > 2) &&
					(varStr.front() == '|') &&
					(varStr.back() == '|'))
				{
					if (GameUtils::getObjectProperty(game, varStr, var2) == true)
					{
						param = VarUtils::toString(var2);
					}
					else
					{
						param = nullText;
					}
				}
				else
				{
					param = varStr;
				}
				Utils::replaceStringInPlace(str, "{" + std::to_string(i) + "}", param);
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
