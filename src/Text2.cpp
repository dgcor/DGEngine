#include "Text2.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

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
		if (format == "[1]")
		{
			text->setText(game.getVariableString(bindings[0]));
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
			text->setText(displayText);
		}
	}
}
