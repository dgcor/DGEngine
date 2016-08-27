#include "Text2.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

std::string Text2::getFormatString(const Game& game,
	const std::vector<std::string>& bindings, const std::string& format)
{
	if (bindings.size() > 0)
	{
		if (format == "[1]")
		{
			return game.getVariableString(bindings[0]);
		}
		else
		{
			std::string displayText = format;
			if (format.size() > 2)
			{
				for (size_t i = 0; i < bindings.size(); i++)
				{
					auto str = game.getVariableString(bindings[i]);
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
	if (bindings.size() > 0)
	{
		text->setText(Text2::getFormatString(game, bindings, format));
	}
}
