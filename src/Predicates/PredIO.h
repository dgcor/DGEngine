#pragma once

#include "Predicate.h"
#include "FileUtils.h"

class PredFileExists : public Predicate
{
private:
	std::string file;

public:
	PredFileExists(const std::string& file_) : file(file_) {}

	virtual Variable getResult(const Game& game) const
	{
		Variable var(file);
		game.getVarOrProp(file, var);
		if (var.is<std::string>() == true)
		{
			return { FileUtils::exists(var.get<std::string>().c_str()) };
		}
		return { false };
	}
};
