#include "ParseFont.h"
#include "FileUtils.h"
#include "Palette.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parsePalette(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}

		std::shared_ptr<Palette> palette;

		if (isValidString(elem, "file") == true)
		{
			palette = std::make_shared<Palette>(elem["file"].GetString());
		}
		else if (isValidString(elem, "palette") == true
			&& isValidString(elem, "trnFile") == true)
		{
			auto refPalette = game.Resources().getPalette(elem["palette"].GetString());
			if (refPalette == nullptr)
			{
				return;
			}
			auto trnFile = FileUtils::readChar(elem["trnFile"].GetString());
			if (trnFile.size() < 256)
			{
				return;
			}
			palette = std::make_shared<Palette>(*refPalette.get(), trnFile);
		}
		if (palette == nullptr)
		{
			return;
		}
		game.Resources().addPalette(elem["id"].GetString(), palette);
	}
}
