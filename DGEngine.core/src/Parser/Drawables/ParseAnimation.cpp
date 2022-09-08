#include "ParseAnimation.h"
#include "Game/Drawables/Animation.h"
#include "Game/Game.h"
#include "ParseDrawable.h"
#include "ParseImage.h"
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/SingleTexturePack.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Animation> getAnimationObj(Game& game, const Value& elem)
	{
		std::shared_ptr<Animation> animation;

		if (isValidString(elem, "texture"))
		{
			auto texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
			if (texture == nullptr)
			{
				return nullptr;
			}
			auto frames = getFramesKey(elem, "frames");
			if ((frames.first == 0 || frames.second == 0) ||
				(frames.first <= 1 && frames.second <= 1))
			{
				animation = std::make_shared<Animation>(*texture);
			}
			else
			{
				TexturePackGroup t;
				t.texture = texture;
				t.animType = getAnimationTypeKey(elem, "animationType");
				auto texPack = std::make_shared<SingleTexturePack>(std::move(t), frames, nullptr);

				if (texPack->size() > 0)
				{
					animation = std::make_shared<Animation>(texPack);
				}
				else
				{
					animation = std::make_shared<Animation>(*texture);
				}
			}
		}
		else if (isValidString(elem, "texturePack"))
		{
			auto texPack = game.Resources().getTexturePack(elem["texturePack"sv].GetStringView());
			if (texPack == nullptr)
			{
				return nullptr;
			}
			auto animInfo = texPack->getAnimation(-1, -1);
			animInfo.indexRange = getFramesKey(elem, "frames", animInfo.indexRange);
			animInfo.animType = getAnimationTypeKey(elem, "animationType", animInfo.animType);
			animation = std::make_shared<Animation>(texPack, animInfo);
		}
		else
		{
			return nullptr;
		}

		parseDrawableProperties(game, elem, *animation);
		parseImageOutlineProperties(elem, *animation);

		animation->Pause(getBoolKey(elem, "pause"));
		auto refresh = animation->getFrameTime();
		if (refresh == sf::Time::Zero)
		{
			refresh = sf::milliseconds(50);
		}
		animation->setFrameTime(getTimeKey(elem, "refresh", refresh));

		return animation;
	}

	static std::shared_ptr<UIObject> parseAnimationFunc(Game& game, const Value& elem)
	{
		return getAnimationObj(game, elem);
	}

	void parseAnimation(Game& game, const Value& elem)
	{
		parseValidIdAndAddDrawable(game, elem, parseAnimationFunc);
	}
}
