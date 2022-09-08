#include "ParseFont.h"
#include "Game/Game.h"
#include "Game/Utils/FileUtils.h"
#include "Game/Utils/GameUtils.h"
#include "Parser/ParseCommon.h"
#include "Parser/Utils/ParseUtils.h"
#include "ParseResource.h"
#include "Resources/Palette.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parsePalette(Game& game, const Value& elem)
	{
		if (parseResourceFromId<std::shared_ptr<Palette>>(game.Resources(), elem) == true)
		{
			return;
		}

		std::string id;
		std::shared_ptr<Palette> palette;

		if (isValidString(elem, "file") == true)
		{
			auto file = elem["file"sv].GetStringView();

			id = parseValidIdOrFilePath(elem, file);
			if (id.empty() == true)
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

			id = parseValidIdOrFilePath(elem, file);
			if (id.empty() == true)
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
