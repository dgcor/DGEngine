#include "ParseLevel.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Json/JsonUtils.h"
#include "ParseLevelAutoMap.h"
#include "ParseLevelMap.h"
#include "Parser/ParseAction.h"
#include "Parser/Drawables/ParseDrawable.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseLevel(Game& game, const Value& elem)
	{
		Document queryDoc;
		const Value* queryObj = nullptr;
		if (isValidString(elem, "load") == true)
		{
			if (JsonUtils::loadFile(elem["load"sv].GetStringView(), queryDoc) == true)
			{
				queryObj = &queryDoc;
			}
		}

		auto id = getStringViewVal(getQueryKey(queryObj, elem, "id"));
		auto level = game.Resources().getLevel<Level>(id);
		if (level == nullptr)
		{
			if (isValidId(id) == false)
			{
				return;
			}

			level = std::make_shared<Level>();

			game.Resources().addDrawable(id, level, true, getStringViewKey(elem, "resource"));
			game.Resources().setCurrentLevel(level);
			level->Id(id);
			level->setShader(game.Shaders().Level);

			auto size = getVector2fKey<sf::Vector2f>(elem, "size", game.DrawRegionSizef());
			parseDrawableProperties(game, elem, *level, size);

			level->Zoom(((float)getIntVal(getQueryKey(queryObj, elem, "zoom"), 100)) / 100.f);
			level->ZoomDrawables(getBoolVal(getQueryKey(queryObj, elem, "zoomDrawables")));
			level->Visible(getBoolVal(getQueryKey(queryObj, elem, "visible"), true));
		}

		parseLevelMap(game, *level, queryObj, elem);
		parseLevelAutomap(game, *level, elem);

		if (elem.HasMember("name"sv) == true)
		{
			level->Name(getStringViewVal(getQueryVal(queryObj, elem["name"sv])));
		}
		if (elem.HasMember("path"sv) == true)
		{
			level->Path(getStringViewVal(getQueryVal(queryObj, elem["path"sv])));
		}
		if (elem.HasMember("followCurrentPlayer"sv) == true)
		{
			auto followCurrentPlayer = getBoolVal(getQueryVal(queryObj, elem["followCurrentPlayer"sv]));
			level->FollowCurrentPlayer(followCurrentPlayer);
		}
		if (elem.HasMember("smoothMovement"sv) == true)
		{
			auto smoothMovement = getBoolVal(getQueryVal(queryObj, elem["smoothMovement"sv]));
			level->setSmoothMovement(smoothMovement);
		}
		if (elem.HasMember("lightRadius"sv) == true)
		{
			level->LightRadius((float)getUIntVal(getQueryVal(queryObj, elem["lightRadius"sv]), 64));
		}

		level->updateView();

		if (elem.HasMember("enableHover"sv))
		{
			bool enableHover = getBoolVal(getQueryVal(queryObj, elem["enableHover"sv]), true);
			level->EnableHover(enableHover);
		}
		if (elem.HasMember("captureInputEvents"sv))
		{
			auto captureInputEvents = getInputEventTypeVal(getQueryVal(queryObj, elem["captureInputEvents"sv]));
			level->setCaptureInputEvents(captureInputEvents);
		}
		if (elem.HasMember("onLeftClick"sv))
		{
			level->setAction(
				str2int16("leftClick"),
				getActionVal(game, getQueryVal(queryObj, elem["onLeftClick"sv]))
			);
		}
		if (elem.HasMember("onRightClick"sv))
		{
			level->setAction(
				str2int16("rightClick"),
				getActionVal(game, getQueryVal(queryObj, elem["onRightClick"sv]))
			);
		}
		if (elem.HasMember("onHoverEnter"sv))
		{
			level->setAction(
				str2int16("hoverEnter"),
				getActionVal(game, getQueryVal(queryObj, elem["onHoverEnter"sv]))
			);
		}
		if (elem.HasMember("onHoverLeave"sv))
		{
			level->setAction(
				str2int16("hoverLeave"),
				getActionVal(game, getQueryVal(queryObj, elem["onHoverLeave"sv]))
			);
		}
		if (elem.HasMember("onScrollDown"sv))
		{
			level->setAction(
				str2int16("scrollDown"),
				getActionVal(game, getQueryVal(queryObj, elem["onScrollDown"sv]))
			);
		}
		if (elem.HasMember("onScrollUp"sv))
		{
			level->setAction(
				str2int16("scrollUp"),
				getActionVal(game, getQueryVal(queryObj, elem["onScrollUp"sv]))
			);
		}
		if (elem.HasMember("experiencePoints"sv))
		{
			const auto& expElem = getQueryVal(queryObj, elem["experiencePoints"sv]);
			if (expElem.IsArray() == true)
			{
				std::vector<uint32_t> expPoints;
				expPoints.push_back(0);
				for (const auto& val : expElem)
				{
					expPoints.push_back(getUIntVal(getQueryVal(queryObj, val)));
				}
				level->setExperiencePoints(expPoints);
			}
		}
	}
}
