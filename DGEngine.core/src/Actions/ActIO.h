#pragma once

#include "Action.h"
#include "FileUtils.h"
#include "Game.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/ParserProperties.h"

class ActDirCopy : public Action
{
private:
	std::string dirSrc;
	std::string dirDst;

public:
	ActDirCopy(const std::string_view dirSrc_, const std::string_view dirDst_)
		: dirSrc(dirSrc_), dirDst(dirDst_) {}

	bool execute(Game& game) override
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
	ActDirCreate(const std::string_view dir_) : dir(dir_) {}

	bool execute(Game& game) override
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
	ReplaceVars replaceVars;

public:
	ActFileCopy(const std::string_view dir_, std::vector<std::string>&& filesRead_,
		const std::string_view fileWrite_, const std::string_view nullText_,
		ReplaceVars replaceVars_) : dir(dir_), filesRead(std::move(filesRead_)),
		fileWrite(fileWrite_), nullText(nullText_), replaceVars(replaceVars_) {}

	bool execute(Game& game) override
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

			if (replaceVars != ReplaceVars::None)
			{
				bool changeValueType = replaceVars == ReplaceVars::Value;
				rapidjson::Document doc;
				if (JsonUtils::loadJsonAndReplaceValues(
					str, doc, game, changeValueType, '|') == true)
				{
					str = JsonUtils::jsonToPrettyString(doc);
				}
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
	ActIODelete(const std::string_view dir_) : dir(dir_) {}

	bool execute(Game& game) override
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
	ActIODeleteAll(const std::string_view dir_, bool deleteRoot_)
		: dir(dir_), deleteRoot(deleteRoot_) {}

	bool execute(Game& game) override
	{
		FileUtils::deleteAll(GameUtils::replaceStringWithVarOrProp(dir, game).c_str(), deleteRoot);
		return true;
	}
};
