#pragma once

#include "Alignment.h"
#include <memory>
#include "Palette.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include "SFML/VertexArray2.h"
#include <string_view>

class TexturePack;

class BitmapFont
{
protected:
	std::shared_ptr<TexturePack> texturePack;
	std::shared_ptr<Palette> palette;
	sf::Color defaultColor{ sf::Color::White };
	int newLine{ 0 };
	int space{ 0 };
	int tab{ 0 };
	int defaultHorizSpaceOffset{ 0 };
	int defaultVertSpaceOffset{ 0 };

	float calculateLineLength(std::string_view::const_iterator it,
		std::string_view::const_iterator itEnd, int horizSpaceOffset) const;

	const Palette* getPalette() const noexcept;
	int getHorizontalSpaceOffset(int horizSpaceOffset) const noexcept;
	int getVerticalSpaceOffset(int vertSpaceOffset) const noexcept;

public:
	BitmapFont(const std::shared_ptr<TexturePack>& texturePack_,
		int16_t newLine_, int16_t space_, int16_t tab_);
	virtual ~BitmapFont() = default;

	int getNewLine() const noexcept { return newLine; }

	void setPalette(const std::shared_ptr<Palette>& pal) noexcept { palette = pal; }
	bool hasPalette() const noexcept { return palette != nullptr; }

	void setColor(const sf::Color& color_) { defaultColor = color_; }
	void setHorizontalSpaceOffset(int horizSpaceOffset) noexcept { defaultHorizSpaceOffset = horizSpaceOffset; }
	void setVerticalSpaceOffset(int vertSpaceOffset) noexcept { defaultVertSpaceOffset = vertSpaceOffset; }

	sf::Vector2f calculateSize(const std::string_view text) const;

	sf::Vector2f calculateSize(const std::string_view text,
		int horizSpaceOffset, int vertSpaceOffset, unsigned* lineCount = nullptr) const;

	void updateVertexString(std::vector<sf::Vertex>& vertexText,
		const std::string_view text, sf::Color color, int horizSpaceOffset,
		int vertSpaceOffset, float sizeX, HorizontalAlign align) const;

	virtual void draw(const VertexArray2& vertexText,
		const sf::Vector2f& pos, const sf::Vector2f& size,
		GameShader* spriteShader, sf::RenderTarget& target) const;
};
