#pragma once

#include "Alignment.h"
#include "Anchor.h"
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>
#include "UIObject.h"

class Game;

namespace GameUtils
{
	const int DoubleClickDelay = 500;

	sf::Vector2f getAlignmentPosition(const sf::Vector2f& position, const sf::Vector2f& size,
		HorizontalAlign horizAlign, VerticalAlign vertAlign);

	sf::Vector2f getAnchorPosition(Anchor srcAnchor, const sf::Vector2f& srcSize,
		const sf::Vector2f& srcPos, const sf::Vector2f& anchorToSize);

	void setAnchorPosSize(Anchor anchor, sf::Vector2f& pos, sf::Vector2f& size,
		const sf::Vector2u& oldWindowSize, const sf::Vector2u& newWindowSize);

	HorizontalAlign getHorizontalAlignment(const std::string& align);

	VerticalAlign getVerticalAlignment(const std::string& align);

	Variable getProperty(const UIObject& uiObject, const unsigned int propHash,
		const std::vector<std::string>& props);

	Variable getProperty(const Game& game, const std::string& str);

	Variable getProperty(const Game& game, const std::string& id, const std::string& props);
}
