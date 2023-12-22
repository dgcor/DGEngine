#include "MakeTexture.h"
#include "Game/Game.h"

std::shared_ptr<sf::Texture> makeTexture(const sf::Vector2u& size, sf::Color color)
{
	sf::Image img;
	img.create(16, 8, sf::Color::Red);

	auto tex = std::make_shared<sf::Texture>();
	tex->loadFromImage(img);
	return tex;
}

std::shared_ptr<sf::Texture> makeTexture(Game& game, const std::string_view id,
	const sf::Vector2u& size, sf::Color color)
{
	auto tex = makeTexture(size, color);
	game.Resources().addTexture(id, tex);
	return tex;
}
