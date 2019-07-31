#include "ParseAnimation.h"
#include "Animation.h"
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
#include "ParseTexture.h"
#include "TexturePacks/SimpleTexturePack.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<Animation> parseAnimationObj(Game& game, const Value& elem)
	{
		std::shared_ptr<Animation> animation;

		if (isValidString(elem, "texture"))
		{
			std::shared_ptr<sf::Texture> texture;
			bool textureAddedToResources = getOrParseTexture(game, elem, "texture", texture);
			if (texture == nullptr)
			{
				return nullptr;
			}
			auto frames = getFramesKey(elem, "frames");
			if (textureAddedToResources == true &&
				((frames.first == 0 || frames.second == 0) ||
				(frames.first <= 1 && frames.second <= 1)))
			{
				animation = std::make_shared<Animation>(*texture);
			}
			else
			{
				sf::Vector2f offset;
				auto animType = getAnimationTypeKey(elem, "animationType");
				auto texPack = std::make_shared<SimpleTexturePack>(
					texture, frames, offset, 0, 0, false,
					animType, nullptr
				);

				if (texPack->size() > 0)
				{
					animation = std::make_shared<Animation>(texPack, frames, animType);
				}
				else
				{
					animation = std::make_shared<Animation>(*texture);
				}
			}
		}
		else if (isValidString(elem, "texturePack"))
		{
			auto texPack = game.Resources().getTexturePack(elem["texturePack"].GetStringStr());
			if (texPack == nullptr)
			{
				return nullptr;
			}
			AnimationType animType;
			auto frames = texPack->getRange(-1, -1, animType);
			frames = getFramesKey(elem, "frames", frames);
			animType = getAnimationTypeKey(elem, "animationType", animType);
			animation = std::make_shared<Animation>(texPack, frames, animType);
		}
		else if (isValidString(elem, "compositeTexture"))
		{
			auto compTex = game.Resources().getCompositeTexture(elem["compositeTexture"].GetStringStr());
			if (compTex == nullptr)
			{
				return nullptr;
			}
			AnimationType animType;
			auto frames = compTex->getRange(-1, -1, animType);
			frames = getFramesKey(elem, "frames", frames);
			animType = getAnimationTypeKey(elem, "animationType", animType);
			animation = std::make_shared<Animation>(compTex, frames, animType);
		}
		else
		{
			return nullptr;
		}

		auto anchor = getAnchorKey(elem, "anchor");
		animation->setAnchor(anchor);
		auto size = animation->Size();
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
		}
		animation->Position(pos);
		animation->Visible(getBoolKey(elem, "visible", true));
		animation->setFrameTime(getTimeKey(elem, "refresh", sf::milliseconds(50)));
		animation->setColor(getColorKey(elem, "color", sf::Color::White));

		auto outline = getColorKey(elem, "outline", sf::Color::Transparent);
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", sf::Color::Transparent);
		animation->setOutline(outline, outlineIgnore);
		animation->setOutlineEnabled(getBoolKey(elem, "enableOutline"));

		return animation;
	}

	void parseAnimation(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		std::string id(elem["id"].GetStringStr());
		if (isValidId(id) == false)
		{
			return;
		}
		auto animation = parseAnimationObj(game, elem);
		if (animation == nullptr)
		{
			return;
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			std::string panelId = getStringVal(elem["panel"]);
			auto panel = game.Resources().getDrawable<Panel>(panelId);
			if (panel != nullptr)
			{
				panel->addDrawable(animation);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, animation, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}
}
