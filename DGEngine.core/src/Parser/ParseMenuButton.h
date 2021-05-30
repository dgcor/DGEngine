#pragma once

#include "Anchor.h"
#include "Font.h"
#include "Json/JsonParser.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include <string_view>

class Game;
class Menu;

namespace Parser
{
	void parseMenuButton(
		Game& game,
		const rapidjson::Value& val,
		::Menu& menu,
		Anchor anchor,
		const sf::Color& color,
		HorizontalAlign horizAlign,
		int horizSpaceOffset,
		int vertSpaceOffset,
		const Font& font,
		unsigned fontSize,
		const sf::SoundBuffer* sound,
		const sf::SoundBuffer* focusSound,
		bool clickUp,
		bool hasFocus,
		bool focusOnClick,
		bool relativePos,
		const sf::Vector2f& origPos,
		const std::string_view resource);
}
