#include "ParseImage.h"
#include "Game.h"
#include "GameUtils.h"
#include "Image.h"
#include "Panel.h"
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
			sf::IntRect rect(0, 0, game.DrawRegionSize().x, game.DrawRegionSize().y);
			image->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}

		if (getBoolKey(elem, "background") == false)
		{
			auto anchor = getAnchorKey(elem, "anchor");
			image->setAnchor(anchor);
			image->Resizable(getBoolKey(elem, "resizable"));
			image->Stretch(getBoolKey(elem, "stretch"));

			auto size = image->Size();
			auto pos = getPositionKey(elem, "position", size, game.RefSize());
			if (getBoolKey(elem, "relativeCoords", true) == true &&
				game.RefSize() != game.DrawRegionSize())
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
			}
			image->Position(pos);
			image->Size(size);
		}
		else
		{
			image->Background(true);
			image->updateSize(game);
		}

		image->Visible(getBoolKey(elem, "visible", true));

		image->setColor(getColorKey(elem, "color", sf::Color::White));

		auto outline = getColorKey(elem, "outline", sf::Color::Transparent);
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", sf::Color::Transparent);
		image->setOutline(outline, outlineIgnore);
		image->setOutlineEnabled(getBoolKey(elem, "enableOutline"));

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			std::string panelId = getStringVal(elem["panel"]);
			auto panel = game.Resources().getDrawable<Panel>(panelId);
			if (panel != nullptr)
			{
				panel->addDrawable(image);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, image, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}
}
