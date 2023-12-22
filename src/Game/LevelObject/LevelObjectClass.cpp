#include "LevelObjectClass.h"
#include "Game/Game.h"
#include "Utils/StringHash.h"

void LevelObjectClass::Id(const std::string_view id_)
{
	id = id_;
	idHash16 = str2int16(id_);
}

std::shared_ptr<Action> LevelObjectClass::getAction(uint16_t nameHash16) const noexcept
{
	for (const auto& elem : actions)
	{
		if (elem.first == nameHash16)
		{
			return elem.second;
		}
	}
	return nullptr;
}

void LevelObjectClass::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action_)
{
	if (nameHash16 == str2int16("") ||
		action_ == nullptr)
	{
		return;
	}
	for (auto& elem : actions)
	{
		if (elem.first == nameHash16)
		{
			elem.second = action_;
			return;
		}
	}
	actions.push_back({ nameHash16, action_ });
}

void LevelObjectClass::executeAction(Game& game, uint16_t nameHash16, bool executeNow) const
{
	for (const auto& elem : actions)
	{
		if (elem.first == nameHash16)
		{
			if (executeNow == true)
			{
				elem.second->execute(game);
			}
			else
			{
				game.Events().addBack(elem.second);
			}
			return;
		}
	}
}
