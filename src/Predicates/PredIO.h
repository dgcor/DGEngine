#pragma once

#include "Predicate.h"
#include "FileUtils.h"
#include "Game.h"

class PredFileExists : public Predicate
{
private:
	std::string file;

public:
	PredFileExists(const std::string& file_) : file(file_) {}

	Variable getResult(const Game& game) const override
	{
		auto file2 = game.getVarOrPropStringS(file);
		if (file2.empty() == false)
		{
			return { FileUtils::exists(file2.c_str()) };
		}
		return { false };
	}
};

class PredGamefileExists : public Predicate
{
private:
	std::string file;

public:
	PredGamefileExists(const std::string& file_) : file(file_) {}

	Variable getResult(const Game& game) const override
	{
		auto file2 = game.getPath() + game.getVarOrPropStringS(file);
		if (file2.empty() == false)
		{
			return { FileUtils::gamefileExists(file2.c_str()) };
		}
		return { false };
	}
};
