#include "Text.h"
#include "Utils/Utils.h"

std::shared_ptr<Action> Text::getAction(uint16_t nameHash16) const noexcept
{
	switch (nameHash16)
	{
	case str2int16("change"):
		return changeAction;
	default:
		return nullptr;
	}
}

bool Text::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("change"):
		changeAction = action;
		break;
	default:
		return false;
	}
	return true;
}

void Text::update(Game& game)
{
	if (triggerOnChange == true)
	{
		triggerOnChange = false;
		if (changeAction != nullptr)
		{
			changeAction->execute(game);
		}
	}
}
