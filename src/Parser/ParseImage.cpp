#include "ParseImage.h"
#include "GameUtils.h"
#include "Image.h"
#include "ParseTexture.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseImage(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false ||
			isValidString(elem, "texture") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}

		std::shared_ptr<sf::Texture> texture;
		if (getOrParseTexture(game, elem, "texture", texture) == false ||
			texture == nullptr)
		{
			return;
		}

		auto image = std::make_shared<Image>(*texture);

		if (elem.HasMember("textureRect"))
		{
			sf::IntRect rect(0, 0, game.WindowSize().x, game.WindowSize().y);
			image->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}

		auto anchor = getAnchorKey(elem, "anchor");
		image->setAnchor(anchor);
		auto size = image->Size();
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		image->Position(pos);
		image->Visible(getBoolKey(elem, "visible", true));

		image->setColor(getColorKey(elem, "color", sf::Color::White));

		auto outline = getColorKey(elem, "outline", sf::Color::Transparent);
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", sf::Color::Transparent);
		image->setOutline(outline, outlineIgnore);
		image->setOutlineEnabled(getBoolKey(elem, "enableOutline"));

		if (isValidString(elem, "resource") == true)
		{
			game.Resources().addDrawable(elem["resource"].GetString(), id, image);
		}
		else
		{
			game.Resources().addDrawable(id, image);
		}
	}
}
