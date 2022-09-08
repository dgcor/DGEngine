#include "ParseImage.h"
#include "Game/Drawables/Image.h"
#include "ParseDrawable.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseImageProperties(Game& game, const Value& elem, Image& image)
	{
		if (elem.HasMember("textureRect"sv))
		{
			sf::IntRect rect(0, 0, game.DrawRegionSize().x, game.DrawRegionSize().y);
			image.setTextureRect(getIntRectVal(elem["textureRect"sv], rect));
		}

		if (getBoolKey(elem, "background") == false)
		{
			image.Resizable(getBoolKey(elem, "resizable", true));
			image.Stretch(getBoolKey(elem, "stretch"));

			parseDrawableProperties(game, elem, image);
		}
		else
		{
			image.Background(true);
			image.updateSize(game);

			image.Visible(getBoolKey(elem, "visible", true));
		}

		parseImageOutlineProperties(elem, image);
	}

	void parseImageOutlineProperties(const Value& elem, Image& image)
	{
		image.setColor(getColorKey(elem, "color", sf::Color::White));

		auto outline = getColorKey(elem, "outline", sf::Color::Transparent);
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", sf::Color::Transparent);
		image.setOutline(outline, outlineIgnore);
		image.setOutlineEnabled(getBoolKey(elem, "enableOutline"));
	}

	static std::shared_ptr<UIObject> parseImageFunc(Game& game, const Value& elem)
	{
		return getImageObj(game, elem);
	}

	void parseImage(Game& game, const Value& elem)
	{
		parseValidIdAndAddDrawable(game, elem, parseImageFunc);
	}
}
