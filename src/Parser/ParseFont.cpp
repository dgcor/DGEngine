#include "ParseFont.h"
#include "ParseTexturePack.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	bool parseFontFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getFont(fromId);
					if (holdsNullFont(obj) == false)
					{
						game.Resources().addFont(id, obj, getStringViewKey(elem, "resource"));
					}
				}
			}
			return true;
		}
		return false;
	}

	bool parseBitmapFont(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return false;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return false;
		}
		auto resource = getStringViewKey(elem, "resource");
		auto texturePackId = getStringKey(elem, "texturePack");
		auto texturePack_ = game.Resources().getTexturePack(texturePackId);
		auto texturePack = std::dynamic_pointer_cast<BitmapFontTexturePack>(texturePack_);
		if (texturePack == nullptr)
		{
			texturePack = parseBitmapFontTexturePackObj(game, elem);
			if (texturePack == nullptr)
			{
				return false;
			}
			if (isValidId(texturePackId) == true)
			{
				game.Resources().addTexturePack(texturePackId, texturePack, resource);
			}
		}

		auto padding = getIntKey(elem, "padding");
		auto font = std::make_shared<BitmapFont>(texturePack, padding);

		if (elem.HasMember("fontPalette") == true &&
			game.Shaders().hasSpriteShader() == true)
		{
			auto palette = game.Resources().getPalette(getStringVal(elem["fontPalette"]));
			font->setPalette(palette);
		}
		if (elem.HasMember("fontColor") == true)
		{
			font->setColor(getColorVal(elem["fontColor"]));
		}

		game.Resources().addFont(id, font, resource);
		return true;
	}

	bool parseFreeTypeFont(Game& game, const Value& elem)
	{
		if (isValidString(elem, "file") == false)
		{
			return false;
		}

		std::string file(elem["file"].GetString());
		std::string id;

		if (isValidString(elem, "id") == true)
		{
			id = elem["id"].GetString();
		}
		else if (getIdFromFile(file, id) == false)
		{
			return false;
		}
		if (isValidId(id) == false)
		{
			return false;
		}

		auto font = std::make_shared<FreeTypeFont>(std::make_shared<sf::PhysFSStream>(file));
		if (font->load() == false)
		{
			return false;
		}

		game.Resources().addFont(id, font, getStringViewKey(elem, "resource"));
		return true;
	}

	void parseFont(Game& game, const Value& elem)
	{
		if (parseFontFromId(game, elem) == true)
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
