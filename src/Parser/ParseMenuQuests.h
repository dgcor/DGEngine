#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseMenuQuests(
		Game& game,
		const rapidjson::Value& elem,
		Menu& menu,
		const sf::Vector2f& pos,
		Anchor anchor,
		const sf::Color& color,
		HorizontalAlign horizAlign,
		int horizSpaceOffset,
		int vertSpaceOffset,
		bool isTextFont,
		const std::shared_ptr<Font2>& font,
		unsigned fontSize,
		const std::shared_ptr<BitmapFont>& bitmapFont,
		const std::shared_ptr<sf::SoundBuffer>& sound,
		const std::shared_ptr<sf::SoundBuffer>& focusSound,
		bool clickUp,
		bool hasFocus,
		bool focusOnClick);
}
