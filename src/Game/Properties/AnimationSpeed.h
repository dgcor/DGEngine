#pragma once

#include <SFML/System/Time.hpp>

struct AnimationSpeed
{
	sf::Time animation;
	sf::Time walk;

	AnimationSpeed() noexcept : animation(sf::microseconds(200000)), walk(sf::microseconds(40000)) {}
	AnimationSpeed(sf::Time animation_, sf::Time walk_) noexcept : animation(animation_), walk(walk_) {}
};
