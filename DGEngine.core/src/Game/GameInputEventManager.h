#pragma once

#include <SFML/Window/Event.hpp>
#include "Utils/UnorderedStringMap.h"
#include <vector>

class GameInputEventManager
{
private:
	UnorderedStringMultiMap<sf::Event> gameInputEvents;

public:
	void clear(const std::string_view key);

	std::vector<sf::Event> get(const std::string_view key) const;

	void set(const std::string& key, const sf::Event& value);
};
