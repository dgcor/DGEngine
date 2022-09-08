#include "ParseFont.h"
#include "Game/Game.h"
#include "Parser/ParseCommon.h"
#include "Parser/Utils/ParseUtils.h"
#include "ParseResource.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	bool cloneFont(Game& game, const Value& elem)
	{
		if (isValidString(elem, "clone") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto cloneId = elem["clone"sv].GetStringView();
				auto id = elem["id"sv].GetStringView();
				if (cloneId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getFont(cloneId);
					if (holdsNullFont(obj) == true)
					{
						return false;
					}
					else if (holdsBitmapFont(obj) == true)
					{
						auto font = std::make_shared<BitmapFont>(*std::get<std::shared_ptr<BitmapFont>>(obj));
						if (elem.HasMember("fontPalette"sv) == true &&
							game.Shaders().hasSpriteShader() == true)
						{
							auto palette = game.Resources().getPalette(getStringViewVal(elem["fontPalette"sv]));
							font->setPalette(palette);
						}
						if (elem.HasMember("fontColor"sv) == true)
						{
							font->setColor(getColorVal(elem["fontColor"sv], sf::Color::White));
						}
						game.Resources().addFont(id, font, getStringViewKey(elem, "resource"));
						return true;
					}
					else
					{
						auto font = std::make_shared<FreeTypeFont>(*std::get<std::shared_ptr<FreeTypeFont>>(obj));
						if (elem.HasMember("fontColor"sv) == true)
						{
							font->setColor(getColorVal(elem["fontColor"sv], sf::Color::White));
						}
						if (elem.HasMember("fontSize"sv) == true)
						{
							font->setCharacterSize(getUIntVal(elem["fontSize"sv]));
						}
						game.Resources().addFont(id, font, getStringViewKey(elem, "resource"));
						return true;
					}
				}
			}
			return true;
		}
		return false;
	}

	bool parseBitmapFont(Game& game, const Value& elem)
	{
		auto id = parseValidId(elem);
		if (id.empty() == true)
		{
			return false;
		}

		auto texturePack = game.Resources().getTexturePack(getStringViewKey(elem, "texturePack"));
		if (texturePack == nullptr)
		{
			return false;
		}

		auto newLine = (int16_t)getIntKey(elem, "newLine", -1);
		auto space = (int16_t)getIntKey(elem, "space", -1);
		auto tab = (int16_t)getIntKey(elem, "tab", -1);

		auto font = std::make_shared<BitmapFont>(texturePack, newLine, space, tab);

		if (elem.HasMember("fontPalette"sv) == true &&
			game.Shaders().hasSpriteShader() == true)
		{
			auto palette = game.Resources().getPalette(getStringViewVal(elem["fontPalette"sv]));
			font->setPalette(palette);
		}
		if (elem.HasMember("fontColor"sv) == true)
		{
			font->setColor(getColorVal(elem["fontColor"sv], sf::Color::White));
		}

		font->setHorizontalSpaceOffset(getIntKey(elem, "horizontalSpaceOffset"));
		font->setVerticalSpaceOffset(getIntKey(elem, "verticalSpaceOffset"));

		game.Resources().addFont(id, font, getStringViewKey(elem, "resource"));
		return true;
	}

	bool parseFreeTypeFont(Game& game, const Value& elem)
	{
		if (isValidString(elem, "file") == false)
		{
			return false;
		}

		auto file = elem["file"sv].GetStringView();
		auto id = parseValidIdOrFilePath(elem, file);

		if (id.empty() == true)
		{
			return false;
		}

		auto font = std::make_shared<FreeTypeFont>(std::make_shared<sf::PhysFSStream>(file));
		if (font->load() == false)
		{
			return false;
		}
		if (elem.HasMember("fontColor"sv) == true)
		{
			font->setColor(getColorVal(elem["fontColor"sv], sf::Color::White));
		}
		if (elem.HasMember("fontSize"sv) == true)
		{
			font->setCharacterSize(getUIntVal(elem["fontSize"sv]));
		}

		game.Resources().addFont(id, font, getStringViewKey(elem, "resource"));
		return true;
	}

	void parseFont(Game& game, const Value& elem)
	{
		if (parseResourceFromId<Font>(game.Resources(), elem) == true)
		{
			return;
		}
		if (cloneFont(game, elem) == true)
		{
			return;
		}
		if (parseBitmapFont(game, elem) == true)
		{
			return;
		}
		parseFreeTypeFont(game, elem);
	}
}
