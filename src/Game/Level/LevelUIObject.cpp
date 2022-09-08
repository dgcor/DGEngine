#include "LevelUIObject.h"
#include "Game/Player/Player.h"
#include "Level.h"
#include "Utils/Utils.h"

std::shared_ptr<Action> LevelUIObject::getAction(const Level& level, uint16_t nameHash16) noexcept
{
	switch (nameHash16)
	{
	case str2int16("click"):
	case str2int16("leftClick"):
		return level.leftAction;
	case str2int16("rightClick"):
		return level.rightAction;
	case str2int16("hoverEnter"):
		return level.hoverEnterAction;
	case str2int16("hoverLeave"):
		return level.hoverLeaveAction;
	case str2int16("scrollDown"):
		return level.scrollDownAction;
	case str2int16("scrollUp"):
		return level.scrollUpAction;
	default:
		return nullptr;
	}
}

bool LevelUIObject::setAction(Level& level, uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("click"):
	case str2int16("leftClick"):
		level.leftAction = action;
		break;
	case str2int16("rightClick"):
		level.rightAction = action;
		break;
	case str2int16("hoverEnter"):
		level.hoverEnterAction = action;
		break;
	case str2int16("hoverLeave"):
		level.hoverLeaveAction = action;
		break;
	case str2int16("scrollDown"):
		level.scrollDownAction = action;
		break;
	case str2int16("scrollUp"):
		level.scrollUpAction = action;
		break;
	default:
		return false;
	}
	return true;
}

bool LevelUIObject::getProperty(const Level& level, const std::string_view prop, Variable& var)
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("clickedObject"):
	{
		if (auto obj = level.levelObjects.ClickedObject())
		{
			return obj->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("currentPlayer"):
	{
		if (auto obj = level.levelObjects.CurrentPlayer())
		{
			return obj->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("hasAutomap"):
		var = Variable(level.hasAutomap());
		return true;
	case str2int16("hasCurrentPlayer"):
		var = Variable(level.levelObjects.hasCurrentPlayer());
		return true;
	case str2int16("hasQuest"):
		var = Variable(level.hasQuest(props.second));
		return true;
	case str2int16("hoverObject"):
	{
		if (auto obj = level.levelObjects.HoverObject())
		{
			return obj->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("id"):
		var = Variable(level.id);
		return true;
	case str2int16("name"):
		var = Variable(level.name);
		return true;
	case str2int16("path"):
		var = Variable(level.path);
		return true;
	case str2int16("levelObject"):
	{
		std::string_view props2;
		auto obj = level.parseLevelObjectIdOrMapPosition(props.second, props2);
		if (obj != nullptr)
		{
			return obj->getProperty(props2, var);
		}
		break;
	}
	case str2int16("player"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto player = level.levelObjects.get<Player>(props2.first);
		if (player != nullptr)
		{
			return player->getProperty(props2.second, var);
		}
		break;
	}
	case str2int16("quest"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		for (const auto& quest : level.quests)
		{
			if (quest.Id() == props2.first)
			{
				return quest.getProperty(props2.second, var);
			}
		}
		break;
	}
	case str2int16("spell"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto spell = level.getSpell(std::string{ props2.first });
		if (spell != nullptr)
		{
			return spell->getProperty(props2.second, var);
		}
		break;
	}
	case str2int16("showAutomap"):
		var = Variable(level.automapSurface.visible);
		return true;
	case str2int16("zoom"):
		var = Variable((double)level.zoomValue.getFinal());
		return true;
	case str2int16("zoomPercentage"):
		var = Variable((int64_t)(std::round(level.zoomValue.getFinal() * 100.f)));
		return true;
	case str2int16("zoomDrawables"):
		var = Variable(level.zoomDrawables);
		return true;
	default:
		return level.getUIObjProp(propHash, props.second, var);
	}
	return false;
}

QueryObject LevelUIObject::getQueryable(const Level& level, const std::string_view prop)
{
	if (prop.empty() == true)
	{
		return &level;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	QueryObject queryObj;
	switch (propHash)
	{
	case str2int16("clickedObject"):
	{
		queryObj = level.levelObjects.ClickedObject();
		break;
	}
	case str2int16("currentPlayer"):
	{
		queryObj = level.levelObjects.CurrentPlayer();
		break;
	}
	break;
	case str2int16("hoverObject"):
	{
		queryObj = level.levelObjects.HoverObject();
		break;
	}
	break;
	case str2int16("player"):
	{
		props = Utils::splitStringIn2(props.second, '.');
		queryObj = level.levelObjects.get<Player>(props.first);
	}
	break;
	case str2int16("quest"):
	{
		props = Utils::splitStringIn2(props.second, '.');
		for (const auto& quest : level.quests)
		{
			if (quest.Id() == props.first)
			{
				queryObj = &quest;
				break;
			}
		}
	}
	break;
	default:
		break;
	}
	auto queryable = getQueryablePtr(queryObj);
	if (queryable != nullptr &&
		props.second.empty() == false)
	{
		return queryable->getQueryable(props.second);
	}
	return queryObj;
}

std::vector<VarOrQueryObject> LevelUIObject::getQueryableList(const Level& level, const std::string_view prop)
{
	std::vector<VarOrQueryObject> queryableList;

	auto props = Utils::splitStringIn2(prop, '.');
	if (props.first.empty() == false)
	{
		if (props.first == "quests")
		{
			for (const auto& quest : level.quests)
			{
				queryableList.push_back(&quest);
			}
		}
		else
		{
			auto player = level.getPlayerOrCurrent(props.first);
			if (player == nullptr)
			{
				player = level.levelObjects.CurrentPlayer();
			}
			if (player != nullptr)
			{
				return player->getQueryableList(props.second);
			}
		}
	}
	return queryableList;
}
