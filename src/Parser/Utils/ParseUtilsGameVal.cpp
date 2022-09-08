#include "ParseUtilsGameVal.h"
#include "Game/Utils/GameUtils.h"
#include "Game/Utils/GameUtils2.h"
#include "Parser/Utils/ParseUtils.h"
#include "ParseUtilsGameKey.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	size_t getInventoryItemIndexVal(const Value& elem, PlayerInventory inv)
	{
		size_t itemIdx = 0;
		if (elem.IsUint() == true)
		{
			itemIdx = elem.GetUint();
		}
		else if (elem.IsString() == true)
		{
			if (inv == PlayerInventory::Body)
			{
				itemIdx = (size_t)GameUtils::getPlayerItemMount(elem.GetStringView());
			}
		}
		return itemIdx;
	}

	InventoryPosition getInventoryPositionVal(const Value& elem, InventoryPosition val)
	{
		if (elem.IsString() == true)
		{
			return GameUtils::getInventoryPosition(elem.GetStringView(), val);
		}
		return val;
	}

	ItemCoordInventory getItemCoordInventoryVal(const Value& elem)
	{
		std::string_view playerId("");
		if (elem.HasMember("player"sv) == true &&
			elem["player"sv].IsString() == true)
		{
			playerId = elem["player"sv].GetStringView();
		}
		PlayerInventory inv = PlayerInventory::Body;
		if (elem.HasMember("inventory"sv) == true)
		{
			inv = getPlayerInventoryVal(elem["inventory"sv]);
			if (elem.HasMember("item"sv) == true)
			{
				if (elem["item"sv].IsArray() == true)
				{
					auto itemPos = getItemXYVal(elem["item"sv]);
					return ItemCoordInventory(playerId, (size_t)inv, itemPos);
				}
				else
				{
					size_t itemIdx = getInventoryItemIndexVal(elem["item"sv], inv);
					return ItemCoordInventory(playerId, (size_t)inv, itemIdx);
				}
			}
			return ItemCoordInventory(playerId, (size_t)inv, 0);
		}
		return ItemCoordInventory(playerId);
	}

	ItemLocation getItemLocationVal(const Value& elem)
	{
		if (elem.HasMember("mapPosition"sv) == true)
		{
			const auto& mapElem = elem["mapPosition"sv];
			if (mapElem.IsArray() == true
				&& mapElem.Size() > 1
				&& mapElem[0].IsNumber() == true
				&& mapElem[1].IsNumber() == true)
			{
				return {
					PairFloat(getNumberVal<float>(mapElem[0]),
					getNumberVal<float>(mapElem[1]))
				};
			}
		}
		return{ getItemCoordInventoryVal(elem) };
	}

	PairUInt8 getItemXYVal(const Value& elem, const PairUInt8& val)
	{
		if (elem.IsArray() == true
			&& elem.Size() > 1
			&& elem[0].IsUint() == true
			&& elem[1].IsUint() == true)
		{
			auto x = elem[0].GetUint();
			auto y = elem[1].GetUint();

			if (x <= 0xFF && y <= 0xFF)
			{
				return PairUInt8((uint8_t)x, (uint8_t)y);
			}
		}
		return val;
	}

	LightSource getLightSourceVal(const Value& elem, LightSource val)
	{
		if (elem.IsUint() == true)
		{
			val.light = elem.GetUint();
		}
		else if (elem.IsArray() == true)
		{
			switch (elem.Size())
			{
			case 2:
			{
				val.light = getUIntVal(elem[0], val.light);
				val.radius = getUIntVal(elem[1], val.radius);
				break;
			}
			case 1:
				val.light = getUIntVal(elem[0], val.light);
				break;
			default:
				break;
			}
		}
		return val;
	}

	AnimationSpeed getPlayerAnimationSpeedVal(const Value& elem)
	{
		AnimationSpeed speed;
		speed.animation = GameUtils::getTime(getIntKey(elem, "animation", 5));
		speed.walk = GameUtils::getTime(getIntKey(elem, "walk", 25));
		return speed;
	}

	PlayerInventory getPlayerInventoryVal(const Value& elem, PlayerInventory val)
	{
		if (elem.IsUint() == true)
		{
			auto idx = elem.GetUint();
			if (idx < (size_t)PlayerInventory::Size)
			{
				return (PlayerInventory)idx;
			}
		}
		else if (elem.IsString() == true)
		{
			return GameUtils::getPlayerInventory(elem.GetStringView(), val);
		}
		return val;
	}
}
