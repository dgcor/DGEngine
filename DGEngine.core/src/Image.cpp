#include "Image.h"
#include "Game.h"
#include "GameUtils.h"
#include "SFML/SFMLUtils.h"
#include "Utils/Utils.h"

Image::Image() noexcept
{
	sprite.setOutlineEnabled(true);
}

Image::Image(const sf::Texture& tex) : sprite(tex)
{
	sprite.setOutlineEnabled(true);
}

Image::Image(const sf::Texture& tex, const std::shared_ptr<Palette>& pal) : sprite(tex, pal)
{
	sprite.setOutlineEnabled(true);
}

Image::Image(const TextureInfo& ti) : sprite(ti)
{
	sprite.setOutlineEnabled(true);
}

Image::Image(const std::vector<TextureInfo>& ti, bool drawAfter) : sprite(ti, drawAfter)
{
	sprite.setOutlineEnabled(true);
}

void Image::setOrigin()
{
	setOrigin(sf::Vector2f(std::round((float)sprite.getTextureRect().width / 2.f),
		std::round((float)sprite.getTextureRect().height / 2.f)));
}

void Image::setTexture(const sf::Texture& texture, bool resetRect)
{
	sprite.setTexture(texture, resetRect);
}

void Image::setTexture(const TextureInfo& ti)
{
	sprite.setTexture(ti);
}

void Image::setTexture(const std::vector<TextureInfo>& ti, bool drawAfter)
{
	sprite.setTexture(ti, drawAfter);
}

void Image::setTexture(const TextureInfoVar& ti)
{
	sprite.setTexture(ti);
}

void Image::updateSize(const Game& game)
{
	if (background == false)
	{
		auto pos = sprite.getPosition();
		auto size = Size();
		GameUtils::setAnchorPosSize(anchor, pos, size, game.OldDrawRegionSize(), game.DrawRegionSize());
		sprite.setPosition(pos);
		Size(size);
	}
	else
	{
		// stretch to fill and keep aspect ratio
		auto scale = SFMLUtils::getScaleToStretchAndKeepAR(
			{ (unsigned)sprite.getTextureRect().width, (unsigned)sprite.getTextureRect().height },
			game.DrawRegionSize()
		);
		sprite.setScale({ scale, scale });

		// center image
		auto newSize = Size();
		sprite.setPosition({
			-std::round(std::abs(newSize.x - (float)game.DrawRegionSize().x) / 2.f),
			-std::round(std::abs(newSize.y - (float)game.DrawRegionSize().y) / 2.f)
		});
	}
}

sf::Vector2f Image::Size() const
{
	auto rect = sprite.getGlobalBounds();
	return sf::Vector2f(rect.width, rect.height);
}

void Image::Size(const sf::Vector2f& size)
{
	if (background == true)
	{
		return;
	}
	if (resizable == true && stretch == true)
	{
		const auto& rect = sprite.getTextureRect();
		sprite.setScale({ size.x / (float)rect.width, size.y / (float)rect.height });
	}
	else
	{
		auto rect = sprite.getTextureRect();
		rect.width = (int)size.x;
		rect.height = (int)size.y;
		sprite.setTextureRect(rect);
	}
}

void Image::draw(const Game& game, sf::RenderTarget& target) const
{
	if (visible)
	{
		sprite.draw(target, game.Shaders().Sprite);
	}
}

bool Image::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return getUIObjProp(str2int16(props.first), props.second, var);
}
