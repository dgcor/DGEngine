#include "GameInputEventManager.h"
#include <ranges>

void GameInputEventManager::clear(const std::string_view key)
{
	auto it = gameInputEvents.find(key);
	if (it != gameInputEvents.end())
	{
		gameInputEvents.erase(it);
	}
}

std::vector<sf::Event> GameInputEventManager::get(const std::string_view key) const
{
	std::vector<sf::Event> inputEvents;

	auto [first, last] = gameInputEvents.equal_range(key);
	for (const auto& [_, v] : std::ranges::subrange(first, last))
	{
		inputEvents.push_back(v);
	}
	return inputEvents;
}

void GameInputEventManager::set(const std::string& key, const sf::Event& value)
{
	gameInputEvents.insert(std::make_pair(key, value));
}
