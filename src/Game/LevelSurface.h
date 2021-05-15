#pragma once

#include <cstdint>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "SFML/VertexArray2.h"
#include "SFML/View2.h"

class LevelObject;
class Panel;
class Sprite2;
struct SpriteShaderCache;
class UIObject;

class LevelSurface
{
private:
	sf::RectangleShape sprite;
	mutable sf::RenderTexture texture;
	View2 mapView{ true };
	View2 drawView{ true };
	bool supportsBigTextures{ false };

	void recreateRenderTexture(bool smoothTexture);
	void recreateRenderTexture(unsigned newWidth, unsigned newHeight, bool smoothTexture);

	void updateVisibleArea();
	void stretchSpriteToZoom(float zoom);

public:
	static constexpr float ZoomMin = 0.5f;
	static constexpr float ZoomMax = 2.0f;

	int32_t tileWidth{ 0 };
	int32_t tileHeight{ 0 };
	int32_t blockWidth{ 0 };	// a tile is 4 blocks
	int32_t blockHeight{ 0 };
	uint32_t subTiles{ 0 };

	sf::FloatRect visibleRect;
	bool visible{ true };

	Anchor getAnchor() const noexcept;
	void setAnchor(const Anchor anchor_) noexcept;

	const sf::Vector2f& Position() const noexcept;
	void Position(const sf::Vector2f& position);
	const sf::Vector2f& Size() const noexcept;
	void Size(const sf::Vector2f& size_);

	const sf::Vector2f& getCenter() const;

	void setCenter(float newViewCenterX, float newViewCenterY);
	void setCenter(const sf::Vector2f& center);
	void setCenter();

	sf::Vector2f getPosition(const sf::Vector2f& point) const;
	sf::Vector2f getDrawPosition(const sf::Vector2f& point) const;
	float getZoom() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
	bool draw(const Game& game, const Panel& obj) const;
	void draw(const Game& game, const UIObject& obj) const;
	void draw(const sf::Drawable& obj, sf::RenderStates states = sf::RenderStates::Default) const;
	void draw(const LevelObject& obj, GameShader* spriteShader, SpriteShaderCache& cache) const;
	void draw(const Sprite2& obj, GameShader* spriteShader, SpriteShaderCache& cache) const;
	void draw(const VertexArray2& obj, const sf::Texture* vertexTexture,
		const Palette* palette, GameShader* spriteShader) const;

	void init(const Game& game);

	void clear(const sf::Color& color) const;

	// newZoom is inverted. numbers < 1 = zoom in and numbers > 1 = zoom out
	bool updateZoom(const Game& game, float newZoom);

	void updateSize(const Game& game);

	void updateView(const Game& game);

	void updateDrawView() const;
	void updateDrawView(const sf::FloatRect& viewportOffset) const;
};
