#include "BitmapText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

BitmapText::BitmapText(const std::shared_ptr<BitmapFont>& font_) : font(font_)
{
	size.y = (float)font->getNewLine();
}

void BitmapText::calculateDrawPosition() noexcept
{
	drawPos = GameUtils::getAlignmentPosition(pos, size, horizAlign, vertAlign);
}

void BitmapText::calculateSize()
{
	size = font->calculateSize(text, horizSpaceOffset, vertSpaceOffset, &lineCount);
}

void BitmapText::setFont(const std::shared_ptr<BitmapFont>& font_)
{
	font = font_;
	calculateSize();
	updateVertexText();
	calculateDrawPosition();
}

void BitmapText::setAnchor(const Anchor anchor_) noexcept
{
	if (anchor != anchor_)
	{
		anchor = anchor_;
		calculateDrawPosition();
	}
}

void BitmapText::updateSize(const Game& game) noexcept
{
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldDrawRegionSize(), game.DrawRegionSize());
	calculateDrawPosition();
}

bool BitmapText::setText(const std::string& utf8Str)
{
	if (text == utf8Str)
	{
		return false;
	}
	text = utf8Str;
	calculateSize();
	updateVertexText();
	calculateDrawPosition();
	return true;
}

void BitmapText::updateVertexText()
{
	font->updateVertexString(
		vertexText.vertices, text, color, horizSpaceOffset,
		vertSpaceOffset, size.x, horizAlign
	);
}

void BitmapText::setColor(const sf::Color& color_) noexcept
{
	color = color_;
	for (auto& vertex : vertexText.vertices)
	{
		vertex.color = color_;
	}
}

void BitmapText::Position(const sf::Vector2f& position) noexcept
{
	pos = position;
	calculateDrawPosition();
}

void BitmapText::setHorizontalAlign(const HorizontalAlign align) noexcept
{
	if (horizAlign != align)
	{
		horizAlign = align;
		calculateDrawPosition();
	}
}

void BitmapText::setVerticalAlign(const VerticalAlign align) noexcept
{
	if (vertAlign != align)
	{
		vertAlign = align;
		calculateDrawPosition();
	}
}

void BitmapText::setHorizontalSpaceOffset(int offset)
{
	if (horizSpaceOffset != offset)
	{
		horizSpaceOffset = offset;
		calculateSize();
		updateVertexText();
		calculateDrawPosition();
	}
}

void BitmapText::setVerticalSpaceOffset(int offset)
{
	if (vertSpaceOffset != offset)
	{
		vertSpaceOffset = offset;
		calculateSize();
		updateVertexText();
		calculateDrawPosition();
	}
}

void BitmapText::draw(const Game& game, sf::RenderTarget& target) const
{
	if (visible == true)
	{
		font->draw(vertexText, drawPos, size, game.Shaders().Sprite, target);
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
	case str2int16("charCount"):
		var = Variable((int64_t)sf::Utf8::count(text.begin(), text.end()));
		break;
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
