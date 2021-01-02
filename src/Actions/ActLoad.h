#pragma once

#include "Action.h"
#include "Game.h"
#include "Parser/ParseFile.h"
#include <vector>

class ActLoad : public Action
{
private:
	std::vector<std::string> args;

public:
	ActLoad(std::vector<std::string>&& args_) : args(std::move(args_)) {}
	ActLoad(const std::string_view file_) { args.push_back(std::string(file_)); }

	bool execute(Game& game) override
	{
		if (args.size() > 0)
		{
			if (args.size() == 1)
			{
				Parser::parseFile(game, args[0]);
			}
			else
			{
				Parser::parseFile(game, args);
			}
		}
		return true;
	}
};

class ActLoadJson : public Action
{
private:
	std::string json;
	std::vector<std::string> args;

public:
	ActLoadJson(const std::string_view json_) : json(json_) {}

	ActLoadJson(const std::string_view json_, std::vector<std::string>&& args_)
		: json(json_), args(std::move(args_)) {}

	bool execute(Game& game) override
	{
		if (args.empty() == true)
		{
			Parser::parseJson(game, json);
		}
		else
		{
			Parser::parseJson(game, json, args);
		}
		return true;
	}
};

class ActLoadRandom : public Action
{
private:
	std::vector<std::string> args;
	std::string endsWith;

public:
	ActLoadRandom(std::vector<std::string>&& args_, const std::string_view endsWith_)
		: args(std::move(args_)), endsWith(endsWith_) {}

	ActLoadRandom(const std::string_view file_, const std::string_view endsWith_)
		: args(), endsWith(endsWith_)
	{
		args.push_back(std::string(file_));
	}

	bool execute(Game& game) override
	{
		if (args.size() > 0)
		{
			auto files = FileUtils::getFileList(args[0], endsWith, true);

			if (files.size() > 0)
			{
				auto idx = Utils::Random::get(files.size() - 1);
				auto file = files[idx];

				if (args.size() == 1)
				{
					Parser::parseFile(game, file);
				}
				else
				{
					auto args2 = args;
					args2[0] = file;
					Parser::parseFile(game, args2);
				}
			}
		}
		return true;
	}
};
