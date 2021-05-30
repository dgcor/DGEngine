#include "Game2.h"
#include "Game/Level.h"
#include "Game/LevelMap.h"

bool Game2::getGameProperty(const std::string_view prop1,
	const std::string_view prop2, Variable& var) const
{
	if (prop1 == "maxLights")
	{
		var = Variable((int64_t)LevelMap::MaxLights());
		return true;
	}
	return Game::getGameProperty(prop1, prop2, var);
}

void Game2::setGameProperty(const std::string_view prop, const Variable& val)
{
	if (prop == "maxLights")
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			LevelMap::MaxLights((uint32_t)std::get<int64_t>(val));
		}
		return;
	}
	Game::setGameProperty(prop, val);
}

bool Game2::getQueryableList(const std::string_view prop1, const std::string_view prop2,
	std::vector<std::variant<const Queryable*, Variable>>& queryableList) const
{
	if (Game::getQueryableList(prop1, prop2, queryableList) == true)
	{
		return true;
	}
	Level* level = nullptr;
	if (prop1 == "currentLevel")
	{
		level = resourceManager.getCurrentLevel<Level>();
	}
	else
	{
		level = resourceManager.getDrawable<Level>(prop1);
	}
	if (level != nullptr && prop2.empty() == false)
	{
		queryableList = level->getQueryableList(prop2);
		return true;
	}
	return false;
}
