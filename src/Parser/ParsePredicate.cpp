#include "ParsePredicate.h"
#include "Predicates/PredIO.h"
#include "Predicates/PredItem.h"
#include "Predicates/PredLevelObject.h"
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
		switch (str2int16(getStringViewVal(elem["name"])))
		{
		case str2int16("file.exists"):
		{
			return std::make_shared<PredFileExists>(getStringKey(elem, "file"));
		}
		case str2int16("gamefile.exists"):
		{
			return std::make_shared<PredGamefileExists>(getStringKey(elem, "file"));
		}
		case str2int16("item.hasRequiredStats"):
		{
			return std::make_shared<PredItemHasRequiredStats>(
				getStringKey(elem, "level"),
				getItemLocationKey(elem, "item"));
		}
		case str2int16("levelObject.isInRange"):
		{
			return std::make_shared<PredLevelObjIsInRange>(
				getStringKey(elem, "level"),
				getStringKey(elem, "object"),
				getIntRectKey(elem, "range"));
		}
		case str2int16("levelObject.isInRangeByClass"):
		{
			return std::make_shared<PredLevelObjIsInRangeByClass>(
				getStringKey(elem, "level"),
				getStringKey(elem, "class"),
				getIntRectKey(elem, "range"));
		}
		case str2int16("player.canUseItem"):
		{
			return std::make_shared<PredPlayerCanUseItem>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"),
				getItemLocationKey(elem, "item"));
		}
		case str2int16("player.canUseSpell"):
		{
			return std::make_shared<PredPlayerCanUseSpell>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"),
				getStringKey(elem, "spell"));
		}
		case str2int16("player.getMaxItemCapacity"):
		{
			return std::make_shared<PredPlayerGetMaxItemCapacity>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"),
				getStringKey(elem, "itemClass"));
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
