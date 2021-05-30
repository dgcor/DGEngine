#include "Text2.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>

namespace
{
	// Add an underline or strikethrough line to the vertex array
	void addLine(sf::VertexArray& vertices, float lineLength, float lineTop, const sf::Color& color, float offset, float thickness, float outlineThickness = 0)
	{
		float top = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
		float bottom = top + std::floor(thickness + 0.5f);

		vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
		vertices.append(sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
		vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
		vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
		vertices.append(sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
		vertices.append(sf::Vertex(sf::Vector2f(lineLength + outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
	}

	// Add a glyph quad to the vertex array
	void addGlyphQuad(sf::VertexArray& vertices, sf::Vector2f position, const sf::Color& color, const sf::Glyph& glyph, float italicShear, float outlineThickness = 0)
	{
		float padding = 1.0;

		float left = glyph.bounds.left - padding;
		float top = glyph.bounds.top - padding;
		float right = glyph.bounds.left + glyph.bounds.width + padding;
		float bottom = glyph.bounds.top + glyph.bounds.height + padding;

		float u1 = static_cast<float>(glyph.textureRect.left) - padding;
		float v1 = static_cast<float>(glyph.textureRect.top) - padding;
		float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width) + padding;
		float v2 = static_cast<float>(glyph.textureRect.top + glyph.textureRect.height) + padding;

		vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * top - outlineThickness, position.y + top - outlineThickness), color, sf::Vector2f(u1, v1)));
		vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italicShear * top - outlineThickness, position.y + top - outlineThickness), color, sf::Vector2f(u2, v1)));
		vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom - outlineThickness, position.y + bottom - outlineThickness), color, sf::Vector2f(u1, v2)));
		vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom - outlineThickness, position.y + bottom - outlineThickness), color, sf::Vector2f(u1, v2)));
		vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italicShear * top - outlineThickness, position.y + top - outlineThickness), color, sf::Vector2f(u2, v1)));
		vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italicShear * bottom - outlineThickness, position.y + bottom - outlineThickness), color, sf::Vector2f(u2, v2)));
	}
}

namespace sf
{
	Text::Text() :
		m_string(),
		m_font(NULL),
		m_characterSize(30),
		m_sizeX(0.f),
		m_letterSpacingFactor(1.f),
		m_lineSpacingFactor(1.f),
		m_style(Regular),
		m_fillColor(255, 255, 255),
		m_outlineColor(0, 0, 0),
		m_outlineThickness(0),
		m_lineCount(0),
		m_vertices(Triangles),
		m_outlineVertices(Triangles),
		m_bounds(),
		m_geometryNeedUpdate(false),
		m_fontTextureId(0)
	{
	}

	Text::Text(const String& string, const Font& font, unsigned int characterSize) :
		m_string(string),
		m_font(&font),
		m_characterSize(characterSize),
		m_sizeX(0.f),
		m_letterSpacingFactor(1.f),
		m_lineSpacingFactor(1.f),
		m_style(Regular),
		m_fillColor(255, 255, 255),
		m_outlineColor(0, 0, 0),
		m_outlineThickness(0),
		m_lineCount(0),
		m_vertices(Triangles),
		m_outlineVertices(Triangles),
		m_bounds(),
		m_geometryNeedUpdate(true),
		m_fontTextureId(0)
	{
	}

	void Text::setString(const String& string)
	{
		if (m_string != string)
		{
			m_string = string;
			m_geometryNeedUpdate = true;
		}
	}

	void Text::setFont(const Font& font)
	{
		if (m_font != &font)
		{
			m_font = &font;
			m_geometryNeedUpdate = true;
		}
	}

	void Text::setCharacterSize(unsigned int size)
	{
		if (m_characterSize != size)
		{
			m_characterSize = size;
			m_geometryNeedUpdate = true;
		}
	}

	void Text::setLetterSpacing(float spacingFactor)
	{
		if (m_letterSpacingFactor != spacingFactor)
		{
			m_letterSpacingFactor = spacingFactor;
			m_geometryNeedUpdate = true;
		}
	}

	void Text::setLineSpacing(float spacingFactor)
	{
		if (m_lineSpacingFactor != spacingFactor)
		{
			m_lineSpacingFactor = spacingFactor;
			m_geometryNeedUpdate = true;
		}
	}

	void Text::setStyle(Uint32 style)
	{
		if (m_style != style)
		{
			m_style = style;
			m_geometryNeedUpdate = true;
		}
	}

	void Text::setFillColor(const Color& color)
	{
		if (color != m_fillColor)
		{
			m_fillColor = color;

			// Change vertex colors directly, no need to update whole geometry
			// (if geometry is updated anyway, we can skip this step)
			if (!m_geometryNeedUpdate)
			{
				for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i)
				{
					m_vertices[i].color = m_fillColor;
				}
			}
		}
	}

	void Text::setOutlineColor(const Color& color)
	{
		if (color != m_outlineColor)
		{
			m_outlineColor = color;

			// Change vertex colors directly, no need to update whole geometry
			// (if geometry is updated anyway, we can skip this step)
			if (!m_geometryNeedUpdate)
			{
				for (std::size_t i = 0; i < m_outlineVertices.getVertexCount(); ++i)
				{
					m_outlineVertices[i].color = m_outlineColor;
				}
			}
		}
	}

	void Text::setOutlineThickness(float thickness)
	{
		if (thickness != m_outlineThickness)
		{
			m_outlineThickness = thickness;
			m_geometryNeedUpdate = true;
		}
	}

	const String& Text::getString() const
	{
		return m_string;
	}

	const Font* Text::getFont() const
	{
		return m_font;
	}

	unsigned int Text::getCharacterSize() const
	{
		return m_characterSize;
	}

	float Text::getLetterSpacing() const
	{
		return m_letterSpacingFactor;
	}

	float Text::getLineSpacing() const
	{
		return m_lineSpacingFactor;
	}

	Uint32 Text::getStyle() const
	{
		return m_style;
	}

	const Color& Text::getFillColor() const
	{
		return m_fillColor;
	}

	const Color& Text::getOutlineColor() const
	{
		return m_outlineColor;
	}

	float Text::getOutlineThickness() const
	{
		return m_outlineThickness;
	}

	Uint32 Text::getLineCount() const
	{
		return m_lineCount;
	}

	Vector2f Text::findCharacterPos(std::size_t index) const
	{
		// Make sure that we have a valid font
		if (!m_font)
		{
			return Vector2f();
		}

		// Adjust the index if it's out of range
		if (index > m_string.getSize())
		{
			index = m_string.getSize();
		}

		// Precompute the variables needed by the algorithm
		bool  isBold = m_style & Bold;
		float whitespaceWidth = m_font->getGlyph(L' ', m_characterSize, isBold).advance;
		float letterSpacing = (whitespaceWidth / 3.f) * (m_letterSpacingFactor - 1.f);
		whitespaceWidth += letterSpacing;
		float lineSpacing = m_font->getLineSpacing(m_characterSize) * m_lineSpacingFactor;

		// Compute the position
		Vector2f position;
		Uint32 prevChar = 0;
		for (std::size_t i = 0; i < index; ++i)
		{
			Uint32 curChar = m_string[i];

			// Apply the kerning offset
			position.x += m_font->getKerning(prevChar, curChar, m_characterSize);
			prevChar = curChar;

			// Handle special characters
			switch (curChar)
			{
			case ' ':  position.x += whitespaceWidth;             continue;
			case '\t': position.x += whitespaceWidth * 4;         continue;
			case '\n': position.y += lineSpacing; position.x = 0; continue;
			}

			// For regular characters, add the advance offset of the glyph
			position.x += m_font->getGlyph(curChar, m_characterSize, isBold).advance + letterSpacing;
		}

		// Transform the position to global coordinates
		position = getTransform().transformPoint(position);

		return position;
	}

	FloatRect Text::getLocalBounds() const
	{
		ensureGeometryUpdate();
		return m_bounds;
	}

	FloatRect Text::getGlobalBounds() const
	{
		return getTransform().transformRect(getLocalBounds());
	}

	void Text::draw(RenderTarget& target, RenderStates states) const
	{
		if (m_font)
		{
			ensureGeometryUpdate();

			states.transform *= getTransform();
			states.texture = &m_font->getTexture(m_characterSize);

			// Only draw the outline if there is something to draw
			if (m_outlineThickness != 0)
			{
				target.draw(m_outlineVertices, states);
			}

			target.draw(m_vertices, states);
		}
	}

	float Text::calculateLineLength(const Uint32* text) const
	{
		if (text[0] == 0 || text[0] == L'\n')
		{
			return 0.f;
		}

		// Compute values related to the text style
		bool  isBold = m_style & Bold;
		float italicShear = (m_style & Italic) ? 0.209f : 0.f; // 12 degrees in radians

		// Precompute the variables needed by the algorithm
		float whitespaceWidth = m_font->getGlyph(L' ', m_characterSize, isBold).advance;
		float letterSpacing = (whitespaceWidth / 3.f) * (m_letterSpacingFactor - 1.f);
		whitespaceWidth += letterSpacing;
		float x = 0.f;

		// Create one quad for each character
		float minX = static_cast<float>(m_characterSize);
		float maxX = 0.f;
		Uint32 prevChar = 0;
		for (std::size_t i = 0; ; ++i)
		{
			Uint32 curChar = text[i];

			if (curChar == 0 || curChar == '\n')
			{
				break;
			}

			// Skip the \r char to avoid weird graphical issues
			if (curChar == '\r')
			{
				continue;
			}

			// Apply the kerning offset
			x += m_font->getKerning(prevChar, curChar, m_characterSize);

			prevChar = curChar;

			// Handle special characters
			if ((curChar == L' ') || (curChar == L'\t'))
			{
				// Update the current bounds (min coordinates)
				minX = std::min(minX, x);

				switch (curChar)
				{
				case L' ':  x += whitespaceWidth;     break;
				case L'\t': x += whitespaceWidth * 4; break;
				}

				// Update the current bounds (max coordinates)
				maxX = std::max(maxX, x);

				// Next glyph, no need to create a quad for whitespace
				continue;
			}

			// Apply the outline
			if (m_outlineThickness != 0)
			{
				const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, isBold, m_outlineThickness);

				float left = glyph.bounds.left;
				float top = glyph.bounds.top;
				float right = glyph.bounds.left + glyph.bounds.width;
				float bottom = glyph.bounds.top + glyph.bounds.height;

				// Update the current bounds with the outlined glyph bounds
				minX = std::min(minX, x + left - italicShear * bottom - m_outlineThickness);
				maxX = std::max(maxX, x + right - italicShear * top - m_outlineThickness);
			}

			// Extract the current glyph's description
			const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, isBold);

			// Update the current bounds with the non outlined glyph bounds
			if (m_outlineThickness == 0)
			{
				float left = glyph.bounds.left;
				float top = glyph.bounds.top;
				float right = glyph.bounds.left + glyph.bounds.width;
				float bottom = glyph.bounds.top + glyph.bounds.height;

				minX = std::min(minX, x + left - italicShear * bottom);
				maxX = std::max(maxX, x + right - italicShear * top);
			}

			// Advance to the next character
			x += glyph.advance + letterSpacing;
		}

		return maxX - minX;
	}

	void Text::calculateSize() const
	{
		// Compute values related to the text style
		bool  isBold = m_style & Bold;
		float italicShear = (m_style & Italic) ? 0.209f : 0.f; // 12 degrees in radians

		// Precompute the variables needed by the algorithm
		float whitespaceWidth = m_font->getGlyph(L' ', m_characterSize, isBold).advance;
		float letterSpacing = (whitespaceWidth / 3.f) * (m_letterSpacingFactor - 1.f);
		whitespaceWidth += letterSpacing;
		float lineSpacing = m_font->getLineSpacing(m_characterSize) * m_lineSpacingFactor;
		float x = 0.f;
		float y = static_cast<float>(m_characterSize);

		// Create one quad for each character
		float minX = static_cast<float>(m_characterSize);
		float minY = static_cast<float>(m_characterSize);
		float maxX = 0.f;
		float maxY = 0.f;
		Uint32 prevChar = 0;
		for (std::size_t i = 0; i < m_string.getSize(); ++i)
		{
			Uint32 curChar = m_string[i];

			// Skip the \r char to avoid weird graphical issues
			if (curChar == '\r')
			{
				continue;
			}

			// Apply the kerning offset
			x += m_font->getKerning(prevChar, curChar, m_characterSize);

			prevChar = curChar;

			// Handle special characters
			if ((curChar == L' ') || (curChar == L'\n') || (curChar == L'\t'))
			{
				// Update the current bounds (min coordinates)
				minX = std::min(minX, x);
				minY = std::min(minY, y);

				switch (curChar)
				{
				case L' ':  x += whitespaceWidth;     break;
				case L'\t': x += whitespaceWidth * 4; break;
				case L'\n': y += lineSpacing; x = 0;  break;
				}

				// Update the current bounds (max coordinates)
				maxX = std::max(maxX, x);
				maxY = std::max(maxY, y);

				// Next glyph, no need to create a quad for whitespace
				continue;
			}

			// Apply the outline
			if (m_outlineThickness != 0)
			{
				const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, isBold, m_outlineThickness);

				float left = glyph.bounds.left;
				float top = glyph.bounds.top;
				float right = glyph.bounds.left + glyph.bounds.width;
				float bottom = glyph.bounds.top + glyph.bounds.height;

				// Update the current bounds with the outlined glyph bounds
				minX = std::min(minX, x + left - italicShear * bottom - m_outlineThickness);
				maxX = std::max(maxX, x + right - italicShear * top - m_outlineThickness);
				minY = std::min(minY, y + top - m_outlineThickness);
				maxY = std::max(maxY, y + bottom - m_outlineThickness);
			}

			// Extract the current glyph's description
			const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, isBold);

			// Update the current bounds with the non outlined glyph bounds
			if (m_outlineThickness == 0)
			{
				float left = glyph.bounds.left;
				float top = glyph.bounds.top;
				float right = glyph.bounds.left + glyph.bounds.width;
				float bottom = glyph.bounds.top + glyph.bounds.height;

				minX = std::min(minX, x + left - italicShear * bottom);
				maxX = std::max(maxX, x + right - italicShear * top);
				minY = std::min(minY, y + top);
				maxY = std::max(maxY, y + bottom);
			}

			// Advance to the next character
			x += glyph.advance + letterSpacing;
		}

		m_sizeX = maxX - minX;
	}

	void Text::ensureGeometryUpdate() const
	{
		if (!m_font)
		{
			return;
		}

		// Do nothing, if geometry has not changed and the font texture has not changed
		if (!m_geometryNeedUpdate && m_font->getTexture(m_characterSize).m_cacheId == m_fontTextureId)
		{
			return;
		}

		// Save the current fonts texture id
		m_fontTextureId = m_font->getTexture(m_characterSize).m_cacheId;

		// Mark geometry as updated
		m_geometryNeedUpdate = false;

		// Clear the previous geometry
		m_vertices.clear();
		m_outlineVertices.clear();
		m_bounds = FloatRect();

		// No text: nothing to draw
		if (m_string.isEmpty())
		{
			m_lineCount = 0;
			return;
		}

		// calculate maximum line size
		calculateSize();
		m_lineCount = 1;

		// Compute values related to the text style
		bool  isBold = m_style & Bold;
		bool  isUnderlined = m_style & Underlined;
		bool  isStrikeThrough = m_style & StrikeThrough;
		bool  isHorizAlignCenter = m_style & HorizontalAlignCenter;
		bool  isHorizAlignRight = m_style & HorizontalAlignRight;
		float italicShear = (m_style & Italic) ? 0.209f : 0.f; // 12 degrees in radians
		float underlineOffset = m_font->getUnderlinePosition(m_characterSize);
		float underlineThickness = m_font->getUnderlineThickness(m_characterSize);

		// Compute the location of the strike through dynamically
		// We use the center point of the lowercase 'x' glyph as the reference
		// We reuse the underline thickness as the thickness of the strike through as well
		FloatRect xBounds = m_font->getGlyph(L'x', m_characterSize, isBold).bounds;
		float strikeThroughOffset = xBounds.top + xBounds.height / 2.f;

		// Precompute the variables needed by the algorithm
		float whitespaceWidth = m_font->getGlyph(L' ', m_characterSize, isBold).advance;
		float letterSpacing = (whitespaceWidth / 3.f) * (m_letterSpacingFactor - 1.f);
		whitespaceWidth += letterSpacing;
		float lineSpacing = m_font->getLineSpacing(m_characterSize) * m_lineSpacingFactor;
		float x = 0.f;
		float y = static_cast<float>(m_characterSize);

		if (isHorizAlignCenter == true)
		{
			x += std::round((m_sizeX / 2.f) - (calculateLineLength(m_string.getData()) / 2.f));
		}
		else if (isHorizAlignRight == true)
		{
			x += (m_sizeX - calculateLineLength(m_string.getData()));
		}

		// Create one quad for each character
		float minX = static_cast<float>(m_characterSize);
		float minY = static_cast<float>(m_characterSize);
		float maxX = 0.f;
		float maxY = 0.f;
		Uint32 prevChar = 0;
		for (std::size_t i = 0; i < m_string.getSize(); ++i)
		{
			Uint32 curChar = m_string[i];

			// Skip the \r char to avoid weird graphical issues
			if (curChar == '\r')
			{
				continue;
			}

			// Apply the kerning offset
			x += m_font->getKerning(prevChar, curChar, m_characterSize);

			// If we're using the underlined style and there's a new line, draw a line
			if (isUnderlined && (curChar == L'\n' && prevChar != L'\n'))
			{
				addLine(m_vertices, x, y, m_fillColor, underlineOffset, underlineThickness);

				if (m_outlineThickness != 0)
				{
					addLine(m_outlineVertices, x, y, m_outlineColor, underlineOffset, underlineThickness, m_outlineThickness);
				}
			}

			// If we're using the strike through style and there's a new line, draw a line across all characters
			if (isStrikeThrough && (curChar == L'\n' && prevChar != L'\n'))
			{
				addLine(m_vertices, x, y, m_fillColor, strikeThroughOffset, underlineThickness);

				if (m_outlineThickness != 0)
				{
					addLine(m_outlineVertices, x, y, m_outlineColor, strikeThroughOffset, underlineThickness, m_outlineThickness);
				}
			}

			prevChar = curChar;

			// Handle special characters
			if ((curChar == L' ') || (curChar == L'\n') || (curChar == L'\t'))
			{
				// Update the current bounds (min coordinates)
				minX = std::min(minX, x);
				minY = std::min(minY, y);

				switch (curChar)
				{
				case L' ':  x += whitespaceWidth;     break;
				case L'\t': x += whitespaceWidth * 4; break;
				case L'\n':
				{
					m_lineCount++;
					y += lineSpacing;
					x = 0;
					if (isHorizAlignCenter == true)
					{
						x += std::round((m_sizeX / 2.f) - (calculateLineLength(m_string.getData() + i + 1) / 2.f));
					}
					else if (isHorizAlignRight == true)
					{
						x += (m_sizeX - calculateLineLength(m_string.getData() + i + 1));
					}
					break;
				}
				}

				// Update the current bounds (max coordinates)
				maxX = std::max(maxX, x);
				maxY = std::max(maxY, y);

				// Next glyph, no need to create a quad for whitespace
				continue;
			}

			// Apply the outline
			if (m_outlineThickness != 0)
			{
				const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, isBold, m_outlineThickness);

				float left = glyph.bounds.left;
				float top = glyph.bounds.top;
				float right = glyph.bounds.left + glyph.bounds.width;
				float bottom = glyph.bounds.top + glyph.bounds.height;

				// Add the outline glyph to the vertices
				addGlyphQuad(m_outlineVertices, Vector2f(x, y), m_outlineColor, glyph, italicShear, m_outlineThickness);

				// Update the current bounds with the outlined glyph bounds
				minX = std::min(minX, x + left - italicShear * bottom - m_outlineThickness);
				maxX = std::max(maxX, x + right - italicShear * top - m_outlineThickness);
				minY = std::min(minY, y + top - m_outlineThickness);
				maxY = std::max(maxY, y + bottom - m_outlineThickness);
			}

			// Extract the current glyph's description
			const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, isBold);

			// Add the glyph to the vertices
			addGlyphQuad(m_vertices, Vector2f(x, y), m_fillColor, glyph, italicShear);

			// Update the current bounds with the non outlined glyph bounds
			if (m_outlineThickness == 0)
			{
				float left = glyph.bounds.left;
				float top = glyph.bounds.top;
				float right = glyph.bounds.left + glyph.bounds.width;
				float bottom = glyph.bounds.top + glyph.bounds.height;

				minX = std::min(minX, x + left - italicShear * bottom);
				maxX = std::max(maxX, x + right - italicShear * top);
				minY = std::min(minY, y + top);
				maxY = std::max(maxY, y + bottom);
			}

			// Advance to the next character
			x += glyph.advance + letterSpacing;
		}

		// If we're using the underlined style, add the last line
		if (isUnderlined && (x > 0))
		{
			addLine(m_vertices, x, y, m_fillColor, underlineOffset, underlineThickness);

			if (m_outlineThickness != 0)
			{
				addLine(m_outlineVertices, x, y, m_outlineColor, underlineOffset, underlineThickness, m_outlineThickness);
			}
		}

		// If we're using the strike through style, add the last line across all characters
		if (isStrikeThrough && (x > 0))
		{
			addLine(m_vertices, x, y, m_fillColor, strikeThroughOffset, underlineThickness);

			if (m_outlineThickness != 0)
			{
				addLine(m_outlineVertices, x, y, m_outlineColor, strikeThroughOffset, underlineThickness, m_outlineThickness);
			}
		}

		// Update the bounding rectangle
		m_bounds.left = minX;
		m_bounds.top = minY;
		m_bounds.width = maxX - minX;
		m_bounds.height = maxY - minY;
	}
}
