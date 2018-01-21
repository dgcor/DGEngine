#include "ParsePredicate.h"
#include "Predicates/PredIO.h"
#include "Predicates/PredItem.h"
#include "Predicates/PredPlayer.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<Predicate> parsePredicateObj(Game& game, const Value& elem)
	{
		if (elem.HasMember("name") == false ||
			elem["name"].IsString() == false)
		{
			return nullptr;
		}
		switch (str2int16(elem["name"].GetString()))
		{
		case str2int16("file.exists"):
		{
			return std::make_shared<PredFileExists>(getStringKey(elem, "file"));
		}
		case str2int16("item.hasRequiredStats"):
		{
			return std::make_shared<PredItemHasRequiredStats>(
				getStringKey(elem, "level"),
				getItemLocationKey(elem, "item"));
		}
		case str2int16("player.canUseItem"):
		{
			return std::make_shared<PredPlayerCanUse>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"),
				getItemLocationKey(elem, "item"));
		}
		case str2int16("player.getMaxGoldCapacity"):
		{
			return std::make_shared<PredPlayerGetMaxGoldCapacity>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"));
		}
		case str2int16("player.hasFreeItemSlot"):
		{
			return std::make_shared<PredPlayerHasFreeItemSlot>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"),
				getItemCoordInventoryKey(elem, "item"));
		}
		default:
			return nullptr;
		}
	}
}
