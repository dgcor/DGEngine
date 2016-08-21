#include "ParseBitmapFont.h"
#include "BitmapFont.h"
#include "FileUtils.h"
#include "ParseTexture.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseBitmapFont(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}

		sf::Image img;
		std::shared_ptr<sf::Texture> texture;

		if (isValidString(elem, "texture"))
		{
			texture = game.Resources().getTexture(elem["texture"].GetString());
			if (texture == nullptr)
			{
				return;
			}
		}
		else if (isValidString(elem, "textureId"))
		{
			img = parseTextureImg(game, elem);
			auto imgSize = img.getSize();
			if (imgSize.x == 0 || imgSize.y == 0)
			{
				return;
			}
			texture = std::make_shared<sf::Texture>();
			texture->loadFromImage(img);
			texture->setSmooth(getBoolKey(elem, "smooth"));
			game.Resources().addTexture(elem["textureId"].GetString(), texture);
		}
		else
		{
			return;
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

		game.Resources().addBitmapFont(elem["id"].GetString(), font);
	}
}
