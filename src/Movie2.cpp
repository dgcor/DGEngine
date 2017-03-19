#include "Movie2.h"
#ifndef USE_SFML_MOVIE_STUB
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

void Movie2::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
{
	switch (nameHash16)
	{
	case str2int16("complete"):
		actionComplete = action;
		return;
	}
}

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

bool Movie2::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return GameUtils::getUIObjProp(*this, str2int32(props.first.c_str()), props.second, var);
}
#endif	// USE_SFML_MOVIE_STUB
