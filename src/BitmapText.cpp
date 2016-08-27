#include "BitmapText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

using Utils::str2int;

void BitmapText::calcDrawPos()
{
	drawPos = GameUtils::getAlignmentPosition(pos, size, horizAlign, vertAlign);
}

void BitmapText::calcSize()
{
	size = font->calculateSize(text, horizSpaceOffset, vertSpaceOffset);
}

void BitmapText::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	calcDrawPos();
}

void BitmapText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == true)
	{
		font->draw(drawPos, text, target, states, color,
			horizSpaceOffset, vertSpaceOffset, size.x, horizAlign);
	}
}

bool BitmapText::getProperty(const std::string& prop, Variable& var) const
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
				var = Variable(text);
				break;
			default:
				return GameUtils::getUIObjProp(*this, propHash, props, var);
			}
			return true;
		}
	}
	return false;
}
