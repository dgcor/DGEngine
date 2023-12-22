#pragma once

#include "AnimationFlags.h"
#include "AnimationType.h"
#include <cstdint>
#include <SFML/System/Time.hpp>
#include <utility>

struct AnimationInfo
{
	std::pair<uint32_t, uint32_t> indexRange;
	AnimationType animType{ AnimationType::Looped };
	AnimationFlags flags{ AnimationFlags::Invalid };
	sf::Time refresh;

	bool Valid() const noexcept { return (int)(flags & AnimationFlags::Valid) != 0; }
	bool Overflow() const noexcept { return (int)(flags & AnimationFlags::Overflow) != 0; }
};
