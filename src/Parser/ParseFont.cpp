#include "ParseFont.h"
#include "Game.h"
#include "ParseTexturePack.h"
#include "TexturePacks/BitmapFontTexturePack.h"
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
				auto fromId = elem["fromId"].GetStringStr();
				auto id = elem["id"].GetStringStr();
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

	bool cloneFont(Game& game, const Value& elem)
	{
		if (isValidString(elem, "clone") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto cloneId = elem["clone"].GetStringStr();
				auto id = elem["id"].GetStringStr();
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
						if (elem.HasMember("fontPalette") == true &&
							game.Shaders().hasSpriteShader() == true)
						{
							auto palette = game.Resources().getPalette(getStringVal(elem["fontPalette"]));
							font->setPalette(palette);
						}
						if (elem.HasMember("fontColor") == true)
						{
							font->setColor(getColorVal(elem["fontColor"], sf::Color::White));
						}
						game.Resources().addFont(id, font, getStringViewKey(elem, "resource"));
						return true;
					}
					else
					{
						auto font = std::make_shared<FreeTypeFont>(*std::get<std::shared_ptr<FreeTypeFont>>(obj));
						if (elem.HasMember("fontColor") == true)
						{
							font->setColor(getColorVal(elem["fontColor"], sf::Color::White));
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
		if (isValidString(elem, "id") == false)
		{
			return false;
		}
		std::string id(elem["id"].GetStringStr());
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
			font->setColor(getColorVal(elem["fontColor"], sf::Color::White));
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

		auto file = elem["file"].GetStringStr();
		std::string id;

		if (isValidString(elem, "id") == true)
		{
			id = elem["id"].GetStringStr();
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
		if (elem.HasMember("fontColor") == true)
		{
			font->setColor(getColorVal(elem["fontColor"], sf::Color::White));
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
