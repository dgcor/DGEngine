#include "StringText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

using Utils::str2int;

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

bool StringText::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() > 1)
	{
		auto props = Utils::splitString(prop, '.');
		if (props.size() > 0)
		{
			auto propHash = str2int(props[0].c_str());
			switch (propHash)
			{
			case str2int("text"):
				var = Variable(text.getString().toAnsiString());
				break;
			default:
				return GameUtils::getUIObjProp(*this, propHash, props, var);
			}
			return true;
		}
	}
	return false;
}
