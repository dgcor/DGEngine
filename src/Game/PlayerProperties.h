#pragma once

#include <cstddef>
#include <SFML/System/Vector2.hpp>

enum class PlayerDirection : size_t
{
	Front,
	FrontLeft,
	Left,
	BackLeft,
	Back,
	BackRight,
	Right,
	FrontRight,
	All,
	Size
};

const size_t PlayerPaletteSize = 8;

enum class PlayerStatus : size_t
{
	Stand1,
	Stand2,
	Walk1,
	Walk2,
	Attack1,
	Attack2,
	Attack3,
	Attack4,
	Defend1,
	Defend2,
	Defend3,
	Defend4,
	Hit1,
	Hit2,
	Die1,
	Die2,
	Size
};

PlayerDirection getPlayerDirection(const sf::Vector2i& currPos, const sf::Vector2i& newPos);
