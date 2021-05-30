#include "StringText.h"
#include "Game.h"
#include "GameUtils.h"
#include <SFML/Config.hpp>
#include "Utils/Utils.h"

StringText::StringText(const std::shared_ptr<FreeTypeFont>& font_, unsigned int characterSize)
	: text({}, *font_)
{
	font = font_;
	charSize = characterSize;
	updateCharSize();
	updateColor();
}

void StringText::calculateDrawPosition()
{
	auto size = Size();
	auto drawPos = GameUtils::getAlignmentPosition(pos, size, horizAlign, vertAlign);
	auto bounds = text.getLocalBounds();
	drawPos.x -= bounds.left;
	drawPos.y -= bounds.top;
	text.setPosition(drawPos);
}

void StringText::setAnchor(const Anchor anchor_)
{
	if (anchor != anchor_)
	{
		anchor = anchor_;
		calculateDrawPosition();
	}
}

void StringText::updateSize(const Game& game)
{
	auto size = Size();
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldDrawRegionSize(), game.DrawRegionSize());
	calculateDrawPosition();
}

std::string StringText::getText() const
{
	std::string utf8Str;
	utf8Str.reserve(text.getString().getSize());
	sf::Utf32::toUtf8(text.getString().begin(), text.getString().end(), std::back_inserter(utf8Str));
	return utf8Str;
}

bool StringText::setText(const std::string& utf8Str)
{
	auto utf32Str = sf::String::fromUtf8(utf8Str.begin(), utf8Str.end());
	if (text.getString() == utf32Str)
	{
		return false;
	}
	text.setString(utf32Str);
	calculateDrawPosition();
	return true;
}

void StringText::setFont(const std::shared_ptr<FreeTypeFont>& font_)
{
	font = font_;
	text.setFont(*font_);
	updateCharSize();
	updateColor();
}

void StringText::updateCharSize()
{
	if (charSize == 0)
	{
		text.setCharacterSize(font->getCharacterSize());
	}
	else
	{
		text.setCharacterSize(charSize);
	}
}

void StringText::setCharacterSize(unsigned int size)
{
	charSize = size;
	updateCharSize();
}

void StringText::setColor(const sf::Color& color_)
{
	color = color_;
	updateColor();
}

void StringText::updateColor()
{
	if (color == sf::Color::White)
	{
		text.setFillColor(font->getColor());
	}
	else
	{
		text.setFillColor(color);
	}
}

void StringText::Position(const sf::Vector2f& position)
{
	pos = position;
	calculateDrawPosition();
}

sf::Vector2f StringText::Size() const
{
	auto bounds = text.getLocalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

void StringText::setHorizontalAlign(const HorizontalAlign align)
{
	if (horizAlign != align)
	{
		horizAlign = align;
		sf::Uint32 style = 0;
		switch (align)
		{
		case HorizontalAlign::Left:
			break;
		case HorizontalAlign::Center:
			style = sf::Text::Style::HorizontalAlignCenter;
			break;
		case HorizontalAlign::Right:
			style = sf::Text::Style::HorizontalAlignRight;
			break;
		default:
			break;
		}
		text.setStyle(style);
		calculateDrawPosition();
	}
}

void StringText::setVerticalAlign(const VerticalAlign align)
{
	if (vertAlign != align)
	{
		vertAlign = align;
		calculateDrawPosition();
	}
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

void StringText::draw(const Game& game, sf::RenderTarget& target) const
{
	if (visible == true)
	{
		target.draw(text);
	}
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
	case str2int16("charCount"):
	case str2int16("length"):
		var = Variable((int64_t)text.getString().getSize());
		break;
	case str2int16("lineCount"):
		var = Variable((int64_t)text.getLineCount());
		break;
	case str2int16("text"):
		var = Variable(getText());
		break;
	default:
		return getUIObjProp(propHash, props.second, var);
	}
	return true;
}
