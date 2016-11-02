#include "ParseAnimation.h"
#include "GameUtils.h"
#include "ParseTexture.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<Animation> parseAnimationObj(Game& game, const Value& elem)
	{
		std::shared_ptr<Animation> animation;

		if (isValidString(elem, "texture"))
		{
			auto texture = game.Resources().getTexture(elem["texture"].GetString());
			if (texture == nullptr)
			{
				return nullptr;
			}
			animation = std::make_shared<Animation>(*texture);

			animation->setFrames(1, getIntKey(elem, "frames", 1));
		}
		else if (isValidString(elem, "id"))
		{
			size_t numFramesX = 1;
			size_t numFramesY = 1;
			auto img = parseTextureImg(game, elem, &numFramesX, &numFramesY);
			auto tex = std::make_shared<sf::Texture>();
			tex->loadFromImage(img);

			game.Resources().addTexture(elem["id"].GetString(), tex);

			animation = std::make_shared<Animation>(*tex);

			animation->setFrames((int16_t)numFramesX, (int16_t)numFramesY);
		}
		else
		{
			return nullptr;
		}

		auto scale = getDoubleKey(elem, "scale", 1.0);
		animation->scale(sf::Vector2f(scale, scale));
		auto anchor = getAnchorKey(elem, "anchor");
		animation->setAnchor(anchor);
		auto pos = getVector2fKey<sf::Vector2f>(elem, "position");
		if (getBoolKey(elem, "relativeCoords", true) == true)
		{
			auto size = animation->Size();
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		animation->Position(pos);
		animation->Visible(getBoolKey(elem, "visible", true));
		animation->setFrameTime(sf::milliseconds(getUIntKey(elem, "refresh", 50)));
		animation->setColor(getColorVar(game, elem, "color", sf::Color::White));

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
		game.Resources().addDrawable(id, animation);
	}
}
