#include "Movie2.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

bool Movie::load()
{
#ifndef DGENGINE_MOVIE_STUB
	if (file == nullptr || file->hasError() == true)
	{
		return false;
	}
	bool ret = movie.openFromStream(*file);
	if (ret == true)
	{
		size = movie.getSize();
	}
	return ret;
#else
	return true;
#endif
}

std::shared_ptr<Action> Movie::getAction(uint16_t nameHash16) const noexcept
{
	switch (nameHash16)
	{
	case str2int16("complete"):
		return actionComplete;
	default:
		return nullptr;
	}
}

bool Movie::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
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

void Movie::draw(const Game& game, sf::RenderTarget& target) const
{
	if (visible == true)
	{
		target.draw(movie);
	}
}

void Movie::updateSize(const Game& game)
{
	auto pos = movie.getPosition();
#ifndef DGENGINE_MOVIE_STUB
	if (size.x <= 0 || size.y <= 0)
	{
		size = movie.getSize();
	}
#else
	auto size = movie.getSize();
#endif
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldDrawRegionSize(), game.DrawRegionSize());
#ifndef DGENGINE_MOVIE_STUB
	movie.fit(sf::FloatRect(pos, size));
#else
	movie.setPosition(pos);
	movie.setSize(size);
#endif
}

void Movie::update(Game& game)
{
	if (visible == false)
	{
		return;
	}

#ifndef DGENGINE_MOVIE_STUB
	movie.update();

	if (movie.getStatus() == sfe::Status::Stopped)
	{
		game.Events().addBack(actionComplete);
	}
#else
	game.Events().addBack(actionComplete);
#endif
}

bool Movie::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return getUIObjProp(str2int16(props.first), props.second, var);
}
