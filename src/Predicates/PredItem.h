#pragma once

#include "Predicate.h"
#include "Game.h"
#include "Game/ItemProperties.h"

class PredItemHasRequiredStats : public Predicate
{
private:
	std::string idLevel;
	ItemLocation itemLocation;

public:
	PredItemHasRequiredStats(const std::string& idLevel_,
		const ItemLocation& itemLocation_)
		: idLevel(idLevel_), itemLocation(itemLocation_) {}

	virtual Variable getResult(const Game& game) const
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			auto item = level->getItem(itemLocation);
			if (item != nullptr)
			{
				if (item->getIntByHash(ItemProp::RequiredStrength) > 0 ||
					item->getIntByHash(ItemProp::RequiredMagic) > 0 ||
					item->getIntByHash(ItemProp::RequiredDexterity) > 0 ||
					item->getIntByHash(ItemProp::RequiredVitality) > 0)
				{
					return { true };
				}
			}
		}
		return { false };
	}
};
