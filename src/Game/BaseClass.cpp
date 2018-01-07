#include "BaseClass.h"
#include "Game.h"

std::shared_ptr<Action> BaseClass::getAction(uint16_t nameHash16) const
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

void BaseClass::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action_)
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
	actions.push_back(std::make_pair(nameHash16, action_));
}

void BaseClass::executeAction(Game& game, uint16_t nameHash16, bool executeNow) const
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
