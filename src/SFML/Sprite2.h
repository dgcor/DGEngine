#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Shader.h"
#include "TextureInfo.h"

struct SpriteShaderCache
{
	sf::Shader* shader{ nullptr };
	Palette* palette{ nullptr };
	sf::Color outline{ sf::Color::Transparent };
	sf::Color ignore{ sf::Color::Transparent };
	sf::Vector2i textureSize;
};

class Sprite2 : private sf::Sprite
{
private:
	sf::Vector2f position;
	sf::Vector2f offset;
	std::shared_ptr<Palette> palette;
	sf::Color outline{ sf::Color::Transparent };
	sf::Color ignore{ sf::Color::Transparent };
	bool outlineEnabled{ false };
	BlendMode blendMode{ BlendMode::Alpha };

	// returns false if shader can be skipped (no palette used, no outline)
	bool needsSpriteShader() const noexcept;

public:
	Sprite2() noexcept : sf::Sprite() {}
	explicit Sprite2(const sf::Texture& texture,
		const std::shared_ptr<Palette>& palette_ = nullptr)
		: sf::Sprite(texture), palette(palette_) {}

	const sf::Vector2f& getDrawPosition() const { return sf::Sprite::getPosition(); }
	const sf::Vector2f& getPosition() const { return position; }
	void setPosition(const sf::Vector2f& position_);
	void setPosition(const sf::Vector2f& position_, const sf::Vector2f& offset_);
	const sf::Vector2f& getOffset() const { return offset; }
	void setOffset(const sf::Vector2f& offset_);

	using sf::Sprite::getColor;
	void setColor(const sf::Color& color);

	const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	void setPalette(const std::shared_ptr<Palette>& pal) noexcept { palette = pal; }
	bool hasPalette() const noexcept { return palette != nullptr; }

	const sf::Color& getOutline() const noexcept { return outline; }
	const sf::Color& getOutlineIgnore() const noexcept { return ignore; }
	void setOutline(const sf::Color& outline_, const sf::Color& ignore_) noexcept;
	bool hasOutline() const noexcept { return outline.a > 0; }
	void setOutlineEnabled(bool enable) noexcept;
	bool isOutlineEnabled() const noexcept { return outlineEnabled; }

	void setTexture(const sf::Texture& texture, bool resetRect = false);
	void setTexture(const TextureInfo& ti, bool resetRect);

	using sf::Sprite::getLocalBounds;
	using sf::Sprite::getGlobalBounds;
	using sf::Sprite::getTexture;
	using sf::Sprite::getTextureRect;
	using sf::Sprite::setTextureRect;
	using sf::Sprite::setScale;
	using sf::Sprite::setOrigin;

	void draw(sf::RenderTarget& target, GameShader* spriteShader,
		SpriteShaderCache* cache = nullptr) const;
};
