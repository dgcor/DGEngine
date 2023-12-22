#pragma once

#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <string_view>

class Game;

std::shared_ptr<sf::Texture> makeTexture(const sf::Vector2u& size = {16, 8}, sf::Color color = sf::Color::Red);

std::shared_ptr<sf::Texture> makeTexture(Game& game, const std::string_view id,
	const sf::Vector2u& size = {16, 8}, sf::Color color = sf::Color::Red);
