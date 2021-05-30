#include "ParseUtilsGameKey.h"
#include "GameUtils2.h"

namespace Parser
{
	using namespace rapidjson;

	size_t getInventoryItemIndexKey(const Value& elem,
		const std::string_view key, PlayerInventory inv)
	{
		if (elem.HasMember(key) == true)
		{
			return getInventoryItemIndexVal(elem[key], inv);
		}
		return 0;
	}

	InventoryPosition getInventoryPositionKey(const Value& elem,
		const std::string_view key, InventoryPosition val)
	{
		if (elem.HasMember(key) == true)
		{
			return getInventoryPositionVal(elem[key], val);
		}
		return val;
	}

	ItemCoordInventory getItemCoordInventoryKey(const Value& elem, const std::string_view key)
	{
		if (elem.HasMember(key) == true)
		{
			return getItemCoordInventoryVal(elem[key]);
		}
		return{};
	}

	ItemLocation getItemLocationKey(const Value& elem, const std::string_view key)
	{
		if (elem.HasMember(key) == true)
		{
			return getItemLocationVal(elem[key]);
		}
		return{};
	}

	PairUInt8 getItemXYKey(const Value& elem, const std::string_view key, const PairUInt8& val)
	{
		if (elem.HasMember(key) == true)
		{
			return getItemXYVal(elem[key], val);
		}
		return val;
	}

	LightSource getLightSourceKey(const rapidjson::Value& elem,
		const std::string_view key, LightSource val)
	{
		if (elem.HasMember(key) == true)
		{
			return getLightSourceVal(elem[key], val);
		}
		return val;
	}

	PlayerAnimation getPlayerAnimationKey(const Value& elem,
		const std::string_view key, PlayerAnimation val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getPlayerAnimation(keyElem.GetStringView(), val);
			}
		}
		return val;
	}

	PlayerDirection getPlayerDirectionKey(const Value& elem,
		const std::string_view key, PlayerDirection val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getPlayerDirection(keyElem.GetStringView(), val);
			}
		}
		return val;
	}

	PlayerInventory getPlayerInventoryKey(const Value& elem,
		const std::string_view key, PlayerInventory val)
	{
		if (elem.HasMember(key) == true)
		{
			return getPlayerInventoryVal(elem[key], val);
		}
		return val;
	}

	PlayerItemMount getPlayerItemMountKey(const Value& elem,
		const std::string_view key, PlayerItemMount val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getPlayerItemMount(keyElem.GetStringView(), val);
			}
		}
		return val;
	}

	PlayerStatus getPlayerStatusKey(const Value& elem,
		const std::string_view key, PlayerStatus val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getPlayerStatus(keyElem.GetStringView(), val);
			}
		}
		return val;
	}
}
