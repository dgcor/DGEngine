#include "ParseFont.h"
#include "FileUtils.h"
#include "Palette.h"
#include "ParseAction.h"
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
						game.Resources().addPalette(id, obj, getStringViewKey(elem, "resource"));
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

		try
		{
			std::string id;
			std::shared_ptr<Palette> palette;

			if (isValidString(elem, "file") == true)
			{
				auto file = getStringViewVal(elem["file"]);

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

				palette = std::make_shared<Palette>(file);
			}
			else if (isValidString(elem, "palette") == true
				&& isValidString(elem, "trnFile") == true)
			{
				auto file = getStringViewVal(elem["trnFile"]);

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
				auto trnFile = FileUtils::readChar(file.data());
				if (trnFile.size() < 256)
				{
					return;
				}
				palette = std::make_shared<Palette>(*refPalette.get(), trnFile);
			}
			else if (isValidString(elem, "clone") == true)
			{
				if (isValidString(elem, "id") == false)
				{
					return;
				}
				std::string clone(elem["clone"].GetString());
				id = elem["id"].GetString();
				if (clone == id || isValidId(id) == false)
				{
					return;
				}
				auto obj = game.Resources().getPalette(clone);
				if (obj == nullptr)
				{
					return;
				}
				palette = std::make_shared<Palette>(*obj);
			}
			else
			{
				return;
			}

			game.Resources().addPalette(id, palette, getStringViewKey(elem, "resource"));
		}
		catch (std::exception ex) {}
	}
}
