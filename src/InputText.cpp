#include "InputText.h"
#include "Game.h"
#include "Utils/Utils.h"

std::shared_ptr<Action> InputText::getAction(uint16_t nameHash16) const noexcept
{
	switch (nameHash16)
	{
	case str2int16("change"):
		return changeAction;
	case str2int16("click"):
	case str2int16("enter"):
		return enterAction;
	case str2int16("minLength"):
		return minLengthAction;
	default:
		return nullptr;
	}
}

bool InputText::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("change"):
		changeAction = action;
		break;
	case str2int16("click"):
	case str2int16("enter"):
		enterAction = action;
		break;
	case str2int16("minLength"):
		minLengthAction = action;
		break;
	default:
		return false;
	}
	return true;
}

void InputText::setRegex(const std::string_view regex_)
{
	regex = Regex(regex_);
}

bool InputText::isValidMin(const std::string& str) const noexcept
{
	if (str.size() < minLength)
	{
		return false;
	}
	if (minValue.has_value() == true)
	{
		if (std::holds_alternative<std::string>(*minValue) == true)
		{
			return std::get<std::string>(*minValue) <= str;
		}
		else if (std::holds_alternative<int64_t>(*minValue) == true)
		{
			return std::get<int64_t>(*minValue) <= Utils::strtoll(str);
		}
		else if (std::holds_alternative<double>(*minValue) == true)
		{
			return std::get<double>(*minValue) <= Utils::strtod(str);
		}
	}
	return true;
}

bool InputText::isValidMax(const std::string& str) const noexcept
{
	if (maxLength > 0 && str.size() > maxLength)
	{
		return false;
	}
	if (maxValue.has_value() == true)
	{
		if (std::holds_alternative<std::string>(*maxValue) == true)
		{
			return std::get<std::string>(*maxValue) >= str;
		}
		else if (std::holds_alternative<int64_t>(*maxValue) == true)
		{
			return std::get<int64_t>(*maxValue) >= Utils::strtoll(str);
		}
		else if (std::holds_alternative<double>(*maxValue) == true)
		{
			return std::get<double>(*maxValue) >= Utils::strtod(str);
		}
	}
	return true;
}

void InputText::click(Game& game)
{
	if (isValidMin(getText()) == false)
	{
		if (minLengthAction != nullptr)
		{
			game.Events().addBack(minLengthAction);
		}
		return;
	}
	if (enterAction != nullptr)
	{
		game.Events().addBack(enterAction);
	}
}

void InputText::update(Game& game)
{
	if (Visible() == false)
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
			auto txt = getText();

			if (ch == 8 && txt.size() > 0) // backspace
			{
				txt.pop_back();
				setText(txt);
			}
			else if (ch < 0 || ch >= 32)
			{
				txt.push_back(ch);

				if (isValidMax(txt) == false)
				{
					break;
				}
				if (regex.match(txt) == false)
				{
					break;
				}
				setText(txt);
			}
			triggerOnChange = true;
			break;
		}
	}
	Text::update(game);
}

bool InputText::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("number"):
		var = Variable((int64_t)std::strtoll(getText().c_str(), nullptr, 10));
		break;
	case str2int16("double"):
		var = Variable((int64_t)std::strtod(getText().c_str(), nullptr));
		break;
	default:
		return Text::getProperty(prop, var);
	}
	return true;
}
