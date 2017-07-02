#include "MovieStub.h"
#ifdef USE_SFML_MOVIE_STUB
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
	auto size = movie.getSize();
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	movie.setPosition(pos);
	movie.setSize(size);
}

void Movie2::update(Game& game)
{
	if (visible == false)
	{
		return;
	}
	game.Events().addBack(actionComplete);
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
#endif	// USE_SFML_MOVIE_STUB
