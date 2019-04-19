#pragma once

#include "Predicate.h"
#include "Game.h"
#include "Game/GameHashes.h"
#include "Game/Level.h"

class PredLevelObjIsInRange : public Predicate
{
private:
	std::string idLevel;
	std::string idObject;
	sf::FloatRect mapLocationRange;

public:
	PredLevelObjIsInRange(const std::string& idLevel_,
		const std::string& idObject_, const sf::FloatRect& mapLocationRange_)
		: idLevel(idLevel_), idObject(idObject_), mapLocationRange(mapLocationRange_) {}

	virtual Variable getResult(const Game& game) const
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto obj = level->getLevelObject(idObject);
			if (obj != nullptr)
			{
				const auto& mapPos = obj->MapPosition();
				if (mapLocationRange.contains(mapPos.x, mapPos.y) == true)
				{
					return { true };
				}
			}
		}
		return { false };
	}
};

class PredLevelObjIsInRangeByClass : public Predicate
{
private:
	std::string idLevel;
	std::string idClass;
	sf::FloatRect mapLocationRange;

public:
	PredLevelObjIsInRangeByClass(const std::string& idLevel_,
		const std::string& idClass_, const sf::FloatRect& mapLocationRange_)
		: idLevel(idLevel_), idClass(idClass_), mapLocationRange(mapLocationRange_) {}

	virtual Variable getResult(const Game& game) const
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto obj = level->getLevelObjectByClass(idClass);
			if (obj != nullptr)
			{
				const auto& mapPos = obj->MapPosition();
				if (mapLocationRange.contains(mapPos.x, mapPos.y) == true)
				{
					return { true };
				}
			}
		}
		return { false };
	}
};
