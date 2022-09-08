#pragma once

#include <memory>
#include <SFML/Graphics/RenderTarget.hpp>

class Game;
class Level;

class LevelDraw
{
public:
	static void draw(const Level& level, const Game& game, sf::RenderTarget& target);
};
