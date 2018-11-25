#pragma once

#include "Alignment.h"
#include "Anchor.h"
#include "AnimationType.h"
#include <cstddef>
#include "Game/GameProperties.h"
#include "IgnoreResource.h"
#include "InputEvent.h"
#include <SFML/System/Vector2.hpp>
#include <string>
#include <string_view>
#include <vector>
#include "UIObject.h"

class Game;

namespace GameUtils
{
	const int DoubleClickDelay = 500;

	AnimationType getAnimationType(const std::string_view str,
		AnimationType val = AnimationType::Looped);

	sf::Vector2f getAlignmentPosition(const sf::Vector2f& position, const sf::Vector2f& size,
		HorizontalAlign horizAlign, VerticalAlign vertAlign) noexcept;

	sf::Vector2f getAnchorPosition(Anchor srcAnchor, const sf::Vector2f& srcSize,
		const sf::Vector2f& srcPos, const sf::Vector2f& anchorToSize) noexcept;

	void setAnchorPosSize(Anchor anchor, sf::Vector2f& pos, sf::Vector2f& size,
		const sf::Vector2u& oldWindowSize, const sf::Vector2u& newWindowSize) noexcept;

	Anchor getAnchor(const std::string_view str, Anchor val);

	HorizontalAlign getHorizontalAlignment(const std::string_view str,
		HorizontalAlign val = HorizontalAlign::Left);

	VerticalAlign getVerticalAlignment(const std::string_view str,
		VerticalAlign val = VerticalAlign::Top);

	sf::Keyboard::Key getKeyCode(int num, sf::Keyboard::Key val) noexcept;

	sf::Keyboard::Key getKeyCode(const std::string_view str, sf::Keyboard::Key val);

	IgnoreResource getIgnoreResource(const std::string_view str, IgnoreResource val);

	InputEvent getInputEvent(const std::string_view str, InputEvent val);

	InventoryPosition getInventoryPosition(const std::string_view str,
		InventoryPosition val = InventoryPosition::TopLeft);

	PlayerDirection getPlayerDirection(const std::string_view str, PlayerDirection val);

	PlayerInventory getPlayerInventory(const std::string_view str,
		PlayerInventory val = PlayerInventory::Body);

	size_t getPlayerInventoryIndex(const std::string_view str,
		PlayerInventory val = PlayerInventory::Body);

	PlayerItemMount getPlayerItemMount(const std::string_view str,
		PlayerItemMount val = PlayerItemMount::LeftHand);

	size_t getPlayerItemMountIndex(const std::string_view str,
		PlayerItemMount val = PlayerItemMount::LeftHand);

	PlayerAnimation getPlayerAnimation(const std::string_view str, PlayerAnimation val);

	PlayerStatus getPlayerStatus(const std::string_view str, PlayerStatus val);

	sf::Time getTime(int fps);

	bool getUIObjProp(const UIObject& uiObject, const uint16_t propHash16,
		const std::string_view prop, Variable& var);

	// gets game's UIObject property. str has no tokens. ex: "game.title"
	bool getObjectProperty(const Game& game, const std::string_view str, Variable& var);

	// replaces "%str%" with obj.getProperty("str")
	std::string replaceStringWithQueryable(const std::string_view str, const Queryable& obj);

	// replaces "%str%" with game.getVarOrProp("str")
	std::string replaceStringWithVarOrProp(const std::string_view str, const Game& obj);
}
