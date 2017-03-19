#include "BitmapText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

void BitmapText::calcDrawPos()
{
	drawPos = GameUtils::getAlignmentPosition(pos, size, horizAlign, vertAlign);
}

void BitmapText::calcSize()
{
	size = font->calculateSize(text, horizSpaceOffset, vertSpaceOffset, &lineCount);
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
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int32(props.first.c_str());
	switch (propHash)
	{
	case str2int32("lineCount"):
		var = Variable((int64_t)lineCount);
		break;
	case str2int32("text"):
		var = Variable(text);
		break;
	default:
		return GameUtils::getUIObjProp(*this, propHash, props.second, var);
	}
	return true;
}
