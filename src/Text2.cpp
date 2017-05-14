#include "Text2.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

void Text2::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
{
	switch (nameHash16)
	{
	case str2int16("change"):
		changeAction = action;
		return;
	}
}

std::string Text2::getFormatString(const Game& game,
	const std::vector<std::string>& bindings, const std::string& format)
{
	if (bindings.size() > 0)
	{
		if (format == "[1]")
		{
			return game.getVarOrPropString(bindings[0]);
		}
		else
		{
			std::string displayText = format;
			if (format.size() > 2)
			{
				for (size_t i = 0; i < bindings.size(); i++)
				{
					auto str = game.getVarOrPropString(bindings[i]);
					Utils::replaceStringInPlace(displayText, "[" + std::to_string(i + 1) + "]", str);
				}
			}
			return displayText;
		}
	}
	return "";
}

void Text2::setBinding(const std::string& binding)
{
	bindings.clear();
	bindings.push_back(binding);
}

void Text2::setBinding(const std::vector<std::string>& bindings_)
{
	bindings = bindings_;
}

void Text2::update(Game& game)
{
	if (text->Visible() == false)
	{
		return;
	}
	if (bindings.size() > 0)
	{
		triggerOnChange = text->setText(Text2::getFormatString(game, bindings, format));
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
