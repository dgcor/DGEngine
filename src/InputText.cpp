#include "InputText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

using Utils::str2int;

void InputText::click(Game& game)
{
	auto txt = text->getText();
	if (txt.size() < minSize && actionMinSize != nullptr)
	{
		game.Events().addBack(actionMinSize);
	}
	else if (actionEnter != nullptr)
	{
		game.Events().addBack(actionEnter);
	}
}

void InputText::update(Game& game)
{
	while (true)
	{
		auto ch = game.getKeyboardChar();
		if (ch != 0)
		{
			auto txt = text->getText();

			if (ch == 8 && txt.size() > 0) // backspace
			{
				txt.pop_back();
				text->setText(txt);
			}
			else if (ch < 0 || ch >= 32)
			{
				if (maxSize > 0 && txt.size() >= maxSize)
				{
					break;
				}

				txt.push_back(ch);
				if (hasRegex == true && std::regex_match(txt, regex) == false)
				{
					break;
				}
				text->setText(txt);
			}
			if (actionChange != nullptr)
			{
				game.Events().addBack(actionChange);
			}
		}
		break;
	}
	text->update(game);
}

bool InputText::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int(props.first.c_str());
	switch (propHash)
	{
	case str2int("text"):
		var = Variable(this->getText());
		break;
	default:
		return GameUtils::getUIObjProp(*this, propHash, props.second, var);
	}
	return true;
}
