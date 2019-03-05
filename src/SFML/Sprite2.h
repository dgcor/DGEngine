#pragma once

#include <memory>
#include "Palette.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "TextureInfo.h"

struct GameShaders;

struct SpriteShaderCache
{
	sf::Shader* shader{ nullptr };
	Palette* palette{ nullptr };
	sf::Color outline{ sf::Color::Transparent };
	sf::Color ignore{ sf::Color::Transparent };
	sf::Vector2i textureSize;
	uint8_t light{ 0 };
};

class Sprite2 : public sf::Sprite
{
private:
	std::shared_ptr<Palette> palette;
	sf::Color outline{ sf::Color::Transparent };
	sf::Color ignore{ sf::Color::Transparent };
	bool outlineEnabled{ false };

	// returns false if shader can be skipped (no palette used, no outline, max light)
	bool needsSpriteShader(uint8_t light) const noexcept;

public:
	Sprite2() noexcept : sf::Sprite() {}
	explicit Sprite2(const sf::Texture& texture,
		const std::shared_ptr<Palette>& palette_ = nullptr)
		: sf::Sprite(texture), palette(palette_) {}

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

	void setTexture(const TextureInfo& ti, bool resetRect);
	using sf::Sprite::setTexture;

	void draw(sf::RenderTarget& target, sf::Shader* spriteShader, uint8_t light = 255) const;

	void draw(sf::RenderTarget& target, sf::Shader* spriteShader,
		SpriteShaderCache& cache, uint8_t light = 255) const;
};
