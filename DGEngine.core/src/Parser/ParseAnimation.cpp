#include "ParseAnimation.h"
#include "Animation.h"
#include <cassert>
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
#include "TexturePacks/SimpleTexturePack.h"
#include "Utils/ParseUtils.h"

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
				MultiTexture t;
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
		animation->Pause(getBoolKey(elem, "pause"));
		auto refresh = animation->getFrameTime();
		if (refresh == sf::Time::Zero)
		{
			refresh = sf::milliseconds(50);
		}
		animation->setFrameTime(getTimeKey(elem, "refresh", refresh));
		animation->setColor(getColorKey(elem, "color", sf::Color::White));

		auto outline = getColorKey(elem, "outline", sf::Color::Transparent);
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", sf::Color::Transparent);
		animation->setOutline(outline, outlineIgnore);
		animation->setOutlineEnabled(getBoolKey(elem, "enableOutline"));

		return animation;
	}

	void parseAnimation(Game& game, const Value& elem,
		const getAnimationObjFuncPtr getAnimationObjFunc)
	{
		assert(getAnimationObjFunc != nullptr);

		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		auto animation = getAnimationObjFunc(game, elem);
		if (animation == nullptr)
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
				panel->addDrawable(animation);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, animation, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}

	void parseAnimation(Game& game, const Value& elem)
	{
		parseAnimation(game, elem, getAnimationObj);
	}
}
