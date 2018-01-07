#include "InputText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

std::shared_ptr<Action> InputText::getAction(uint16_t nameHash16)
{
	switch (nameHash16)
	{
	case str2int16("change"):
		return actionChange;
	case str2int16("click"):
	case str2int16("enter"):
		return actionEnter;
	case str2int16("minSize"):
		return actionMinSize;
	default:
		return nullptr;
	}
}

bool InputText::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
{
	switch (nameHash16)
	{
	case str2int16("change"):
		actionChange = action;
		break;
	case str2int16("click"):
	case str2int16("enter"):
		actionEnter = action;
		break;
	case str2int16("minSize"):
		actionMinSize = action;
		break;
	default:
		return false;
	}
	return true;
}

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
	if (text->Visible() == false)
	{
		return;
	}
	if (game.wasTextEntered() == true &&
		game.TextEntered().unicode < 256 &&
		game.TextEntered().unicode != 0)
	{
		while (true)
		{
			auto ch = static_cast<char>(game.TextEntered().unicode);
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
			break;
		}
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
	auto propHash = str2int16(props.first.c_str());
	switch (propHash)
	{
	case str2int16("text"):
		var = Variable(this->getText());
		break;
	default:
		return GameUtils::getUIObjProp(*this, propHash, props.second, var);
	}
	return true;
}
