#pragma once

#include "Action.h"
#include "FileUtils.h"
#include "Game.h"
#include "GameUtils.h"

class ActDirCopy : public Action
{
private:
	std::string dirSrc;
	std::string dirDst;

public:
	ActDirCopy(const std::string& dirSrc_, const std::string& dirDst_)
		: dirSrc(dirSrc_), dirDst(dirDst_) {}

	virtual bool execute(Game& game)
	{
		FileUtils::copyDir(GameUtils::replaceStringWithVarOrProp(dirSrc, game).c_str(),
			GameUtils::replaceStringWithVarOrProp(dirDst, game).c_str());
		return true;
	}
};

class ActDirCreate : public Action
{
private:
	std::string dir;

public:
	ActDirCreate(const std::string& dir_) : dir(dir_) {}

	virtual bool execute(Game& game)
	{
		FileUtils::createDir(GameUtils::replaceStringWithVarOrProp(dir, game).c_str());
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
			auto str = FileUtils::readText(
				GameUtils::replaceStringWithVarOrProp(fileRead, game).c_str());

			std::string param;
			Variable var2;

			for (size_t i = 1; i < filesRead.size(); i++)
			{
				const auto& varStr = filesRead[i];

				if ((varStr.size() > 2) &&
					(varStr.front() == '%') &&
					(varStr.back() == '%'))
				{
					if (game.getVarOrProp(varStr, var2) == true)
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
				Utils::replaceStringInPlace(str, "{" + Utils::toString(i) + "}", param);
			}

			auto writePath = GameUtils::replaceStringWithVarOrProp(dir, game);
			if (writePath.size() > 0 && Utils::endsWith(writePath, "/") == false)
			{
				writePath += '/';
			}
			if (fileWrite.size() > 0)
			{
				writePath += game.getVarOrPropStringS(fileWrite);
			}
			else
			{
				writePath += game.getVarOrPropStringS(FileUtils::getFileName(fileRead));
			}
			FileUtils::saveText(writePath, str);
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
		FileUtils::deleteFile(GameUtils::replaceStringWithVarOrProp(dir, game).c_str());
		return true;
	}
};

class ActIODeleteAll : public Action
{
private:
	std::string dir;
	bool deleteRoot;

public:
	ActIODeleteAll(const std::string& dir_, bool deleteRoot_)
		: dir(dir_), deleteRoot(deleteRoot_) {}

	virtual bool execute(Game& game)
	{
		FileUtils::deleteAll(GameUtils::replaceStringWithVarOrProp(dir, game).c_str(), deleteRoot);
		return true;
	}
};
