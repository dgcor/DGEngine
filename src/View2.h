#pragma once

#include <SFML/Graphics/View.hpp>

class Game;

class View2 : public sf::View
{
private:
	sf::Vector2f position;

public:
	void setViewport(const Game& game);

	const sf::Vector2f& getPosition() const { return position; }
	void setPosition(const sf::Vector2f& position_) { position = position_; }
};
