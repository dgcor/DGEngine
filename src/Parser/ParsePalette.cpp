#include "ParseFont.h"
#include "FileUtils.h"
#include "Palette.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	bool parsePaletteFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getPalette(fromId);
					if (obj != nullptr)
					{
						game.Resources().addPalette(id, obj);
					}
				}
			}
			return true;
		}
		return false;
	}

	void parsePalette(Game& game, const Value& elem)
	{
		if (parsePaletteFromId(game, elem) == true)
		{
			return;
		}

		if (isValidString(elem, "file") == true)
		{
			std::string file(elem["file"].GetString());
			std::string id;

			if (isValidString(elem, "id") == true)
			{
				id = elem["id"].GetString();
			}
			else if (getIdFromFile(file, id) == false)
			{
				return;
			}
			if (isValidId(id) == false)
			{
				return;
			}
			if (game.Resources().hasPalette(id) == true)
			{
				return;
			}

			auto palette = std::make_shared<Palette>(file);
			game.Resources().addPalette(id, palette);
		}
		else if (isValidString(elem, "palette") == true
			&& isValidString(elem, "trnFile") == true)
		{
			std::string file(elem["trnFile"].GetString());
			std::string id;

			if (isValidString(elem, "id") == true)
			{
				id = elem["id"].GetString();
			}
			else if (getIdFromFile(file, id) == false)
			{
				return;
			}
			if (isValidId(id) == false)
			{
				return;
			}
			if (game.Resources().hasPalette(id) == true)
			{
				return;
			}

			auto refPalette = game.Resources().getPalette(elem["palette"].GetString());
			if (refPalette == nullptr)
			{
				return;
			}
			auto trnFile = FileUtils::readChar(file.c_str());
			if (trnFile.size() < 256)
			{
				return;
			}
			auto palette = std::make_shared<Palette>(*refPalette.get(), trnFile);
			game.Resources().addPalette(id, palette);
		}
	}
}
