#pragma once

#include "Alignment.h"
#include "Anchor.h"
#include "Game/GameProperties.h"
#include "IgnoreResource.h"
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

	Anchor getAnchor(const std::string& str, Anchor val);

	HorizontalAlign getHorizontalAlignment(const std::string& str);

	VerticalAlign getVerticalAlignment(const std::string& str);

	sf::Keyboard::Key getKeyCode(int num, sf::Keyboard::Key val);

	sf::Keyboard::Key getKeyCode(const std::string& str, sf::Keyboard::Key val);

	IgnoreResource getIgnoreResource(const std::string& str, IgnoreResource val);

	InventoryPosition getInventoryPosition(const std::string& str,
		InventoryPosition val = InventoryPosition::TopLeft);

	PlayerDirection getPlayerDirection(const std::string& str, PlayerDirection val);

	PlayerInventory getPlayerInventory(const std::string& str,
		PlayerInventory val = PlayerInventory::Body);

	size_t getPlayerInventoryIndex(const std::string& str,
		PlayerInventory val = PlayerInventory::Body);

	PlayerItemMount getPlayerItemMount(const std::string& str,
		PlayerItemMount val = PlayerItemMount::LeftHand);

	size_t getPlayerItemMountIndex(const std::string& str,
		PlayerItemMount val = PlayerItemMount::LeftHand);

	PlayerStatus getPlayerStatus(const std::string& str, PlayerStatus val);

	bool getUIObjProp(const UIObject& uiObject, const uint16_t propHash16,
		const std::string& prop, Variable& var);

	bool getObjectProperty(const Game& game, const std::string& str, Variable& var);

	std::string replaceStringWithQueryable(const std::string& str, const Queryable& obj);
}
