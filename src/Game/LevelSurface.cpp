#include "LevelSurface.h"
#include "Game.h"
#include "LevelObject.h"
#include "Panel.h"
#include "Utils/Utils.h"

Anchor LevelSurface::getAnchor() const noexcept
{
	return mapView.getAnchor();
}

void LevelSurface::setAnchor(const Anchor anchor_) noexcept
{
	mapView.setAnchor(anchor_);
	drawView.setAnchor(anchor_);
}

const sf::Vector2f& LevelSurface::Position() const noexcept
{
	return mapView.getPosition();
}

void LevelSurface::Position(const sf::Vector2f& position)
{
	mapView.setPosition(position);
	drawView.setPosition(position);
	sprite.setPosition(position);
}

const sf::Vector2f& LevelSurface::Size() const noexcept
{
	return mapView.getSize();
}

void LevelSurface::Size(const sf::Vector2f& size_)
{
	mapView.setSize(size_);
	drawView.setSize(size_);
	sprite.setSize(drawView.getRoundedSize());
}

const sf::Vector2f& LevelSurface::getCenter() const
{
	return mapView.getCenter();
}

void LevelSurface::setCenter(float newViewCenterX, float newViewCenterY)
{
	mapView.setCenter(newViewCenterX, newViewCenterY);
	drawView.setCenter(newViewCenterX, newViewCenterY);
	updateVisibleArea();
}

void LevelSurface::setCenter(const sf::Vector2f& center)
{
	setCenter(center.x, center.y);
}

void LevelSurface::setCenter()
{
	setCenter(
		std::round(mapView.getRoundedSize().x / 2.f),
		std::round(mapView.getRoundedSize().y / 2.f)
	);
}

sf::Vector2f LevelSurface::getPosition(const sf::Vector2f& point) const
{
	return mapView.getPosition(point);
}

void LevelSurface::recreateRenderTexture(bool smoothTexture)
{
	if (supportsBigTextures == true && mapView.getZoom() > 1.f)
	{
		recreateRenderTexture(
			(unsigned)(drawView.getSize().x * ZoomMax),
			(unsigned)(drawView.getSize().y * ZoomMax),
			smoothTexture
		);
	}
	else
	{
		recreateRenderTexture(
			(unsigned)drawView.getSize().x,
			(unsigned)drawView.getSize().y,
			smoothTexture
		);
	}
}

void LevelSurface::recreateRenderTexture(unsigned newWidth, unsigned newHeight, bool smoothTexture)
{
	auto texSize = texture.getSize();
	if ((texSize.x != newWidth || texSize.y != newHeight) &&
		newWidth > 0 && newHeight > 0)
	{
		texture.create(newWidth, newHeight);
		texture.setSmooth(smoothTexture);
		texture.setRepeated(true);
		sprite.setTexture(&texture.getTexture(), true);
	}
}

void LevelSurface::updateVisibleArea()
{
	const auto& viewCenter = mapView.getCenter();
	const auto& viewSize = mapView.getVisibleSize();
	visibleRect.left = viewCenter.x - std::round(viewSize.x / 2.f);
	visibleRect.top = viewCenter.y - std::round(viewSize.y / 2.f);
	visibleRect.width = std::round(viewSize.x);
	visibleRect.height = std::round(viewSize.y);
}

void LevelSurface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == true)
	{
		texture.display();
		target.draw(sprite, states);
	}
}

bool LevelSurface::draw(const Game& game, const Panel& obj) const
{
	return obj.draw(game, texture, visibleRect);
}

void LevelSurface::draw(const Game& game, const UIObject& obj) const
{
	return obj.draw(game, texture);
}

void LevelSurface::draw(const sf::Drawable& obj, sf::RenderStates states) const
{
	texture.draw(obj, states);
}

void LevelSurface::draw(const LevelObject& obj, GameShader* spriteShader,
	SpriteShaderCache& cache) const
{
	obj.draw(texture, spriteShader, cache);
}

void LevelSurface::draw(const Sprite2& obj, GameShader* spriteShader,
	SpriteShaderCache& cache) const
{
	obj.draw(texture, spriteShader, &cache);
}

void LevelSurface::draw(const VertexArray2& obj, const sf::Texture* vertexTexture,
	const Palette* palette, GameShader* spriteShader) const
{
	obj.draw(vertexTexture, palette, spriteShader, texture);
}

void LevelSurface::init(const Game& game)
{
	auto maxTexSize = std::max(
		sf::VideoMode::getDesktopMode().width * 2,
		sf::VideoMode::getDesktopMode().height * 2
	);
	supportsBigTextures = (sf::Texture::getMaximumSize() >= maxTexSize);
	updateVisibleArea();
	recreateRenderTexture(game.SmoothScreen());
}

void LevelSurface::clear(const sf::Color& color) const
{
	texture.clear(color);
}

bool LevelSurface::updateZoom(const Game& game, float newZoom)
{
	newZoom = Utils::round(newZoom, 4);
	if (newZoom != mapView.getZoom())
	{
		mapView.setZoom(newZoom);
		if (newZoom <= 1.f || supportsBigTextures == false)
		{
			drawView.setZoom(std::max(newZoom, 1.f));
		}
		else
		{
			// zoom is inverted in views, so max = min
			drawView.setZoom(ZoomMax);
		}
		updateView(game);
		return true;
	}
	return false;
}

void LevelSurface::stretchSpriteToZoom(float zoom)
{
	if (zoom > 1.f)
	{
		auto size = texture.getSize();
		if (supportsBigTextures == false)
		{
			sprite.setTextureRect({ 0, 0, (int)size.x, (int)size.y });
			return;
		}
		sf::IntRect textureRect(
			(int)std::round(((float)size.x / 2.f) - (drawView.getSize().x / 2.f) * zoom),
			(int)std::round(((float)size.y / 2.f) - (drawView.getSize().y / 2.f) * zoom),
			(int)std::round(drawView.getSize().x * zoom),
			(int)std::round(drawView.getSize().y * zoom)
		);
		sprite.setTextureRect(textureRect);
	}
	else
	{
		auto size = texture.getSize();
		auto sizeDiffX = (size.x - drawView.getSize().x) / 2.f * zoom;
		auto sizeDiffY = (size.y - drawView.getSize().y) / 2.f * zoom;
		sf::IntRect textureRect(
			(int)std::round(((size.x - visibleRect.width) / 2.f) - sizeDiffX),
			(int)std::round(((size.y - visibleRect.height) / 2.f) - sizeDiffY),
			(int)std::round(visibleRect.width * (size.x / drawView.getSize().x)),
			(int)std::round(visibleRect.height * (size.y / drawView.getSize().y))
		);
		sprite.setTextureRect(textureRect);
	}
}

void LevelSurface::updateSize(const Game& game)
{
	mapView.updateSize(game, true);
	drawView.updateSize(game, true);
	sprite.setPosition(drawView.getPosition());
	sprite.setSize(drawView.getRoundedSize());

	updateVisibleArea();
	recreateRenderTexture(game.SmoothScreen());
	stretchSpriteToZoom(mapView.getZoom());
}

void LevelSurface::updateView(const Game& game)
{
	mapView.update(game, true);
	drawView.update(game, true);

	updateVisibleArea();
	recreateRenderTexture(game.SmoothScreen());
	stretchSpriteToZoom(mapView.getZoom());
}

void LevelSurface::updateDrawView() const
{
	texture.setView(drawView.getView());
}

void LevelSurface::updateDrawView(const sf::FloatRect& viewportOffset) const
{
	if (viewportOffset == sf::FloatRect(0.f, 0.f, 0.f, 0.f) ||
		mapView.getZoom() != 1.f)
	{
		texture.setView(drawView.getView());
	}
	else
	{
		auto newView = drawView.getView();
		sf::Vector2f newPosition(
			{ drawView.getPosition().x + viewportOffset.left,
			drawView.getPosition().y + viewportOffset.top }
		);
		newView.setSize(
			{ newView.getSize().x + viewportOffset.width,
			newView.getSize().y + viewportOffset.height }
		);

		float top = ((newPosition.x - drawView.getPosition().x) / drawView.getSize().x);
		float left = ((newPosition.y - drawView.getPosition().y) / drawView.getSize().y);
		float width = (newView.getSize().x / drawView.getSize().x);
		float height = (newView.getSize().y / drawView.getSize().y);

		newView.setViewport({ top, left, width, height });
		texture.setView(newView);
	}
}
