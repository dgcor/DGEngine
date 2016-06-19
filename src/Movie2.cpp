#include "Movie2.h"
#ifndef USE_SFML_MOVIE_STUB
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

void Movie2::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == true)
	{
		target.draw(movie, states);
	}
}

void Movie2::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	auto pos = movie.getPosition();
	if (size.x <= 0 || size.y <= 0)
	{
		size = movie.getSize();
	}
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	movie.fit(sf::FloatRect(pos, size));
}

void Movie2::update(Game& game)
{
	if (visible == false)
	{
		return;
	}

	movie.update();

	if (movie.getStatus() == sfe::Status::Stopped)
	{
		game.Events().addBack(actionComplete);
	}
}

Variable Movie2::getProperty(const std::string& prop) const
{
	if (prop.size() > 1)
	{
		auto props = Utils::splitString(prop, '.');
		if (props.size() > 0)
		{
			return GameUtils::getProperty(*this, Utils::str2int(props[0].c_str()), props);
		}
	}
	return Variable();
}
#endif	// USE_SFML_MOVIE_STUB
