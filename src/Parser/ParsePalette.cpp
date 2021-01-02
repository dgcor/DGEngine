#include "ParseFont.h"
#include "FileUtils.h"
#include "Game.h"
#include "GameUtils.h"
#include "Palette.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	bool parsePaletteFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto fromId = elem["fromId"sv].GetStringView();
				auto id = elem["id"sv].GetStringView();
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

		std::string id;
		std::shared_ptr<Palette> palette;

		if (isValidString(elem, "file") == true)
		{
			auto file = elem["file"sv].GetStringView();

			if (isValidString(elem, "id") == true)
			{
				id = elem["id"sv].GetStringView();
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

			Palette::ColorFormat colorFormat = Palette::ColorFormat::RGB;

			if (isValidString(elem, "colorFormat") == true)
			{
				colorFormat = GameUtils::getColorFormat(getStringViewVal(elem["colorFormat"sv]));
			}

			palette = std::make_shared<Palette>(file, colorFormat);
		}
		else if (isValidString(elem, "palette") == true
			&& isValidString(elem, "trnFile") == true)
		{
			auto file = getStringViewVal(elem["trnFile"sv]);

			if (isValidString(elem, "id") == true)
			{
				id = elem["id"sv].GetStringView();
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

			auto refPalette = game.Resources().getPalette(elem["palette"sv].GetStringView());
			if (refPalette == nullptr)
			{
				return;
			}
			auto trnFile = FileUtils::readChar(file.data());
			auto trnStart = std::min(getUIntKey(elem, "trnStart"), 0x7FFFFFFFu);
			auto trnLength = std::min(getUIntKey(elem, "trnLength", 256), 256u);
			palette = std::make_shared<Palette>(
				*refPalette.get(), trnFile, trnStart, trnLength
			);
		}
		else if (isValidString(elem, "clone") == true)
		{
			if (isValidString(elem, "id") == false)
			{
				return;
			}
			auto clone = elem["clone"sv].GetStringView();
			id = elem["id"sv].GetStringView();
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
}
