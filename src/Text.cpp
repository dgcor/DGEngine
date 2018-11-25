#include "Text.h"
#include "Game.h"
#include "GameUtils.h"
#include "TextUtils.h"
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

void Text::setBinding(const std::string& binding)
{
	bindings.clear();
	bindings.push_back(binding);
}

void Text::setBinding(std::vector<std::string> bindings_)
{
	bindings = std::move(bindings_);
}

void Text::update(Game& game)
{
	if (text->Visible() == false)
	{
		return;
	}
	if (bindings.size() > 0)
	{
		triggerOnChange = text->setText(TextUtils::getFormatString(game, format, bindings));
	}
	if (triggerOnChange == true)
	{
		triggerOnChange = false;
		if (changeAction != nullptr)
		{
			changeAction->execute(game);
		}
	}
}
