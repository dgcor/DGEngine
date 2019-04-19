#include "BitmapText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

void BitmapText::calculateDrawPosition() noexcept
{
	drawPos = GameUtils::getAlignmentPosition(pos, size, horizAlign, vertAlign);
}

void BitmapText::calculateSize()
{
	size = font->calculateSize(text, horizSpaceOffset, vertSpaceOffset, &lineCount);
}

void BitmapText::updateSize(const Game& game) noexcept
{
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldDrawRegionSize(), game.DrawRegionSize());
	calculateDrawPosition();
}

void BitmapText::draw(const Game& game, sf::RenderTarget& target) const
{
	if (visible == true)
	{
		font->draw(drawPos, text, game, target, color,
			horizSpaceOffset, vertSpaceOffset, size.x, horizAlign);
	}
}

bool BitmapText::getProperty(const std::string_view prop, Variable& var) const
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
		var = Variable((int64_t)text.size());
		break;
	case str2int16("lineCount"):
		var = Variable((int64_t)lineCount);
		break;
	case str2int16("text"):
		var = Variable(text);
		break;
	default:
		return getUIObjProp(propHash, props.second, var);
	}
	return true;
}
