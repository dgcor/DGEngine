#include "ParseCelFile.h"
#include "Cel.h"
#include "ParseUtils.h"
#include "Utils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<CelFile> parseCelFileObj(Game& game, const rapidjson::Value& elem)
	{
		if (isValidString(elem, "file") == false)
		{
			return nullptr;
		}

		std::string fileName = elem["file"].GetString();
		bool isCl2 = Utils::endsWith(fileName, "cl2");

		auto celFile = std::make_shared<CelFile>(fileName.c_str(), isCl2, false);
		if (celFile->Size() == 0)
		{
			return nullptr;
		}
		return celFile;
	}

	void parseCelFile(Game& game, const Value& elem)
	{
		auto celFile = parseCelFileObj(game, elem);
		if (celFile == nullptr)
		{
			return;
		}
		game.Resources().addCelFile(elem["id"].GetString(), celFile);
	}
}
