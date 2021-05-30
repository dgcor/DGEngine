#include "ParseImage.h"
#include <cassert>
#include "Game.h"
#include "GameUtils.h"
#include "Image.h"
#include "Panel.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Image> getImageObj(Game& game, const Value& elem)
	{
		std::shared_ptr<Image> image;

		if (isValidString(elem, "texture"))
		{
			auto texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
			if (texture == nullptr)
			{
				return nullptr;
			}
			image = std::make_shared<Image>(*texture);
		}
		else if (isValidString(elem, "texturePack"))
		{
			auto texPack = game.Resources().getTexturePack(elem["texturePack"sv].GetStringView());
			if (texPack == nullptr)
			{
				return nullptr;
			}
			TextureInfoVar tiVar;
			if (texPack->get(getUIntKey(elem, "textureIndex"), tiVar) == false)
			{
				return nullptr;
			}
			if (std::holds_alternative<TextureInfo>(tiVar) == true)
			{
				image = std::make_shared<Image>(std::get<TextureInfo>(tiVar));
			}
			else
			{
				image = std::make_shared<Image>(std::get<std::vector<TextureInfo>>(tiVar));
			}
		}
		else
		{
			return nullptr;
		}

		if (elem.HasMember("textureRect"sv))
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

		return image;
	}

	void parseImage(Game& game, const Value& elem,
		const getImageObjFuncPtr getImageObjFunc)
	{
		assert(getImageObjFunc != nullptr);

		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		auto image = getImageObjFunc(game, elem);
		if (image == nullptr)
		{
			return;
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			auto panelId = getStringViewVal(elem["panel"sv]);
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

	void parseImage(Game& game, const Value& elem)
	{
		parseImage(game, elem, getImageObj);
	}
}
