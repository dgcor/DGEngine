#include "Movie2.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

std::shared_ptr<Action> Movie2::getAction(uint16_t nameHash16)
{
	switch (nameHash16)
	{
	case str2int16("complete"):
		return actionComplete;
	default:
		return nullptr;
	}
}

bool Movie2::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
{
	switch (nameHash16)
	{
	case str2int16("complete"):
		actionComplete = action;
		break;
	default:
		return false;
	}
	return true;
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
#ifndef USE_SFML_MOVIE_STUB
	if (size.x <= 0 || size.y <= 0)
	{
		size = movie.getSize();
	}
#else
	auto size = movie.getSize();
#endif
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
#ifndef USE_SFML_MOVIE_STUB
	movie.fit(sf::FloatRect(pos, size));
#else
	movie.setPosition(pos);
	movie.setSize(size);
#endif
}

void Movie2::update(Game& game)
{
	if (visible == false)
	{
		return;
	}

#ifndef USE_SFML_MOVIE_STUB
	movie.update();

	if (movie.getStatus() == sfe::Status::Stopped)
	{
		game.Events().addBack(actionComplete);
	}
#else
	game.Events().addBack(actionComplete);
#endif
}

bool Movie2::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return GameUtils::getUIObjProp(*this, str2int16(props.first.c_str()), props.second, var);
}
