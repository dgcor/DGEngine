#include "ParsePlayerActions.h"
#include "Game/Actions/ActPlayer.h"
#include "Game/Utils/GameUtils2.h"
#include "Parser/Utils/ParseUtils.h"
#include "Parser/Utils/ParseUtilsGameKey.h"

namespace Parser2::Actions
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parsePlayerAddItemQuantity(const Value& elem, bool remove)
	{
		return std::make_shared<ActPlayerAddItemQuantity>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "itemClass"),
			getInventoryPositionKey(elem, "position"),
			getVariableKey(elem, "value"),
			remove);
	}

	std::shared_ptr<Action> parsePlayerAddItemQuantity(const Value& elem)
	{
		return parsePlayerAddItemQuantity(elem, getBoolKey(elem, "remove"));
	}

	std::shared_ptr<Action> parsePlayerAddToProperty(const Value& elem)
	{
		return std::make_shared<ActPlayerAddToProperty>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "property"),
			getVariableKey(elem, "value"),
			getBoolKey(elem, "remove"));
	}

	std::shared_ptr<Action> parsePlayerMove(const Value& elem)
	{
		return std::make_shared<ActPlayerMove>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			getVector2UnsignedNumberKey<PairFloat, float>(elem, "position"),
			getBoolKey(elem, "resetDirection"),
			getBoolKey(elem, "smooth"));
	}

	std::shared_ptr<Action> parsePlayerRemoveItemQuantity(const Value& elem)
	{
		return parsePlayerAddItemQuantity(elem, true);
	}

	std::shared_ptr<Action> parsePlayerSelectSpell(const Value& elem)
	{
		return std::make_shared<ActPlayerSelectSpell>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "spell"));
	}

	std::shared_ptr<Action> parsePlayerSetDefaultSpeed(const Value& elem)
	{
		auto speed = getPlayerAnimationSpeedVal(elem);
		if (elem.HasMember("animation"sv) == false)
		{
			speed.animation = sf::Time::Zero;
		}
		if (elem.HasMember("walk"sv) == false)
		{
			speed.walk = sf::Time::Zero;
		}
		return std::make_shared<ActPlayerSetDefaultSpeed>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			speed);
	}

	std::shared_ptr<Action> parsePlayerSetDirection(const Value& elem)
	{
		return std::make_shared<ActPlayerSetDirection>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			getPlayerDirectionKey(elem, "direction"));
	}

	std::shared_ptr<Action> parsePlayerSetProperty(const Value& elem)
	{
		return std::make_shared<ActPlayerSetProperty>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "property"),
			getVariableKey(elem, "value"));
	}

	std::shared_ptr<Action> parsePlayerSetRestStatus(const Value& elem)
	{
		return std::make_shared<ActPlayerSetRestStatus>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			(uint8_t)getUIntKey(elem, "status"));
	}

	std::shared_ptr<Action> parsePlayerSetTextureIndex(const Value& elem)
	{
		return std::make_shared<ActPlayerSetTextureIndex>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			getUIntKey(elem, "index"));
	}

	std::shared_ptr<Action> parsePlayerWalk(const Value& elem)
	{
		return std::make_shared<ActPlayerWalk>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			getPlayerDirectionKey(elem, "direction"),
			getBoolKey(elem, "executeAction"));
	}

	std::shared_ptr<Action> parsePlayerWalkToClick(const Value& elem)
	{
		return std::make_shared<ActPlayerWalkToClick>(
			getStringViewKey(elem, "player"),
			getStringViewKey(elem, "level"),
			getBoolKey(elem, "executeAction", true));
	}
}
