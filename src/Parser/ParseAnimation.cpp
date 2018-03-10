#include "ParseAnimation.h"
#include "GameUtils.h"
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
				auto texPack = std::make_shared<SimpleTexturePack>(
					texture, frames, offset, 0, false, nullptr, false);

				if (texPack->size() > 0)
				{
					animation = std::make_shared<Animation>(texPack, frames);
				}
				else
				{
					animation = std::make_shared<Animation>(*texture);
				}
			}
		}
		else if (isValidString(elem, "texturePack"))
		{
			auto texPack = game.Resources().getTexturePack(elem["texturePack"].GetString());
			if (texPack == nullptr)
			{
				return nullptr;
			}
			auto frames = std::make_pair(0u, texPack->size() - 1);
			frames = getFramesKey(elem, "frames", frames);
			animation = std::make_shared<Animation>(texPack, frames);
		}
		else
		{
			return nullptr;
		}

		auto scale = getFloatKey(elem, "scale", 1.0);
		animation->scale(sf::Vector2f(scale, scale));
		auto anchor = getAnchorKey(elem, "anchor");
		animation->setAnchor(anchor);
		auto size = animation->Size();
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
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
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}
		auto animation = parseAnimationObj(game, elem);
		if (animation == nullptr)
		{
			return;
		}
		if (isValidString(elem, "resource") == true)
		{
			game.Resources().addDrawable(elem["resource"].GetString(), id, animation);
		}
		else
		{
			game.Resources().addDrawable(id, animation);
		}
	}
}
