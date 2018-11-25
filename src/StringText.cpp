#include "StringText.h"
#include <algorithm>
#include "Game.h"
#include "GameUtils.h"
#include <SFML/Config.hpp>
#include "Utils/Utils.h"

void StringText::calculateLineCount()
{
	if (text.getString().isEmpty() == true)
	{
		lineCount = 0;
	}
	else
	{
		lineCount = std::count(text.getString().begin(), text.getString().end(), '\n') + 1;
	}
}

void StringText::calculateDrawPosition()
{
	auto size = Size();
	auto drawPos = GameUtils::getAlignmentPosition(pos, size, horizAlign, vertAlign);
	text.setPosition(drawPos);
}

void StringText::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	auto size = Size();
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	calculateDrawPosition();
}

bool StringText::setText(const std::string& str)
{
	if (text.getString() == str)
	{
		return false;
	}
	text.setString(str);
	calculateLineCount();
	calculateDrawPosition();
	return true;
}

void StringText::setHorizontalSpaceOffset(int offset) noexcept
{
	float factor = 1.f;
	if (offset != 0)
	{
		factor = (float)(text.getCharacterSize() + offset) / (float)text.getCharacterSize();
	}
	text.setLetterSpacing(factor);
	calculateDrawPosition();
}

void StringText::setVerticalSpaceOffset(int offset) noexcept
{
	float factor = 1.f;
	if (offset != 0)
	{
		factor = (float)(text.getCharacterSize() + offset) / (float)text.getCharacterSize();
	}
	text.setLineSpacing(factor);
	calculateDrawPosition();
}

bool StringText::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("length"):
		var = Variable((int64_t)text.getString().getSize());
		break;
	case str2int16("lineCount"):
		var = Variable((int64_t)lineCount);
		break;
	case str2int16("text"):
		var = Variable(text.getString().toAnsiString());
		break;
	default:
		return GameUtils::getUIObjProp(*this, propHash, props.second, var);
	}
	return true;
}
