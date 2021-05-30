#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/String.hpp>
#include <string>
#include <vector>

namespace sf
{
	// based on SFML's Text class
	class Text : public Drawable, public Transformable
	{
	private:
		String m_string;
		const Font* m_font;
		unsigned int m_characterSize;
		mutable float m_sizeX;
		float m_letterSpacingFactor;
		float m_lineSpacingFactor;
		Uint32 m_style;
		Color m_fillColor;
		Color m_outlineColor;
		float m_outlineThickness;
		mutable Uint32 m_lineCount;
		mutable VertexArray m_vertices;
		mutable VertexArray m_outlineVertices;
		mutable FloatRect m_bounds;
		mutable bool m_geometryNeedUpdate;
		mutable Uint64 m_fontTextureId;

		void draw(RenderTarget& target, RenderStates states) const override;

		float calculateLineLength(const Uint32* text) const;
		void calculateSize() const;
		void ensureGeometryUpdate() const;

	public:
		enum Style
		{
			Regular = 0,
			Bold = 1 << 0,
			Italic = 1 << 1,
			Underlined = 1 << 2,
			StrikeThrough = 1 << 3,
			HorizontalAlignCenter = 1 << 4,
			HorizontalAlignRight = 1 << 5
		};

		Text();
		Text(const String& string, const Font& font, unsigned int characterSize = 30);

		void setString(const String& string);

		void setFont(const Font& font);

		void setCharacterSize(unsigned int size);

		void setLineSpacing(float spacingFactor);

		void setLetterSpacing(float spacingFactor);

		void setStyle(Uint32 style);

		void setFillColor(const Color& color);

		void setOutlineColor(const Color& color);

		void setOutlineThickness(float thickness);

		const String& getString() const;

		const Font* getFont() const;

		unsigned int getCharacterSize() const;

		float getLetterSpacing() const;

		float getLineSpacing() const;

		Uint32 getStyle() const;

		const Color& getFillColor() const;

		const Color& getOutlineColor() const;

		float getOutlineThickness() const;

		Uint32 getLineCount() const;

		Vector2f findCharacterPos(std::size_t index) const;

		FloatRect getLocalBounds() const;

		FloatRect getGlobalBounds() const;
	};
}
