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
	if (lineSpacing > 0.f)
	{
		size = font->calculateSize(text, lineSpacing);
	}
	else
	{
		lineSpacing = 0.f;
		size = font->calculateSize(text);
	}
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
		font->draw(drawPos, text, target, states, color, lineSpacing, size.x, horizAlign);
	}
}

Variable BitmapText::getProperty(const std::string& prop) const
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
				return Variable(text);
				break;
			default:
				return GameUtils::getProperty(*this, propHash, props);
			}
		}
	}
	return Variable();
}
