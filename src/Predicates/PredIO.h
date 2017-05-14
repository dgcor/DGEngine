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
		auto file2 = game.getVarOrPropString(file);
		if (file2.empty() == false)
		{
			return { FileUtils::exists(file2.c_str()) };
		}
		return { false };
	}
};
