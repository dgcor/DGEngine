#pragma once

#include "AnimationType.h"
#include <cstdint>
#include <SFML/System/Time.hpp>
#include <utility>

struct AnimationInfo
{
	std::pair<uint32_t, uint32_t> indexRange;
	AnimationType animType{ AnimationType::Looped };
	sf::Time refresh;
};
