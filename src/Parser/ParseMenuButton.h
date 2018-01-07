#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

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
		bool isTextFont,
		const std::shared_ptr<Font2>& font,
		unsigned fontSize,
		const std::shared_ptr<BitmapFont>& bitmapFont,
		sf::SoundBuffer* sound,
		sf::SoundBuffer* focusSound,
		bool clickUp,
		bool hasFocus,
		bool focusOnClick,
		bool relativePos,
		const sf::Vector2f& origPos);
}
