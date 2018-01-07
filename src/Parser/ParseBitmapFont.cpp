#include "ParseBitmapFont.h"
#include "BitmapFont.h"
#include "FileUtils.h"
#include "ParseTexture.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseBitmapFont(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}
		if (isValidString(elem, "texture") == false)
		{
			return;
		}

		sf::Image img;
		std::string textureId = elem["texture"].GetString();
		auto texture = game.Resources().getTexture(textureId);
		if (texture == nullptr)
		{
			if (isValidId(textureId) == false)
			{
				return;
			}
			img = parseTextureImg(game, elem);
			auto imgSize = img.getSize();
			if (imgSize.x == 0 || imgSize.y == 0)
			{
				return;
			}
			texture = parseTextureObj(game, elem, img);
			if (texture == nullptr)
			{
				return;
			}
			game.Resources().addTexture(textureId, texture);
		}

		auto rows = getIntKey(elem, "rows", 16);
		auto cols = getIntKey(elem, "cols", 16);
		auto padding = getIntKey(elem, "padding");
		bool isVertical = getStringKey(elem, "direction") == "vertical";

		std::shared_ptr<BitmapFont> font;

		if (elem.HasMember("charSizeFile"))
		{
			auto charSizes = FileUtils::readChar(elem["charSizeFile"].GetString(), 258);
			font = std::make_shared<BitmapFont>(texture, rows, cols, padding, isVertical, charSizes);
		}
		else
		{
			if (img.getSize().x == 0)
			{
				img = texture->copyToImage();
			}
			font = std::make_shared<BitmapFont>(texture, rows, cols, padding, isVertical, img);
		}

		game.Resources().addBitmapFont(id, font);
	}
}
