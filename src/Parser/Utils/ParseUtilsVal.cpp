#include "ParseUtilsVal.h"
#include <cctype>
#include "FileUtils.h"
#include "GameUtils.h"
#include "SFMLUtils.h"
#include "Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace Utils;

	bool getBoolVal(const Value& elem, bool val)
	{
		if (elem.IsBool() == true)
		{
			return elem.GetBool();
		}
		return val;
	}

	double getDoubleVal(const Value& elem, double val)
	{
		if (elem.IsDouble() == true)
		{
			return elem.GetDouble();
		}
		return val;
	}

	float getFloatVal(const Value& elem, float val)
	{
		if (elem.IsFloat() == true)
		{
			return elem.GetFloat();
		}
		return val;
	}

	int getIntVal(const Value& elem, int val)
	{
		if (elem.IsInt() == true)
		{
			return elem.GetInt();
		}
		return val;
	}

	int64_t getInt64Val(const Value& elem, int64_t val)
	{
		if (elem.IsInt64() == true)
		{
			return elem.GetInt64();
		}
		return val;
	}

	const char* getStringCharVal(const Value& elem, const char* val)
	{
		if (elem.IsString() == true)
		{
			return elem.GetString();
		}
		return val;
	}

	std::string getStringVal(const Value& elem, const std::string& val)
	{
		return getStringCharVal(elem, val.c_str());
	}

	unsigned getUIntVal(const Value& elem, unsigned val)
	{
		if (elem.IsUint() == true)
		{
			return elem.GetUint();
		}
		return val;
	}

	uint64_t getUInt64Val(const Value& elem, uint64_t val)
	{
		if (elem.IsUint64() == true)
		{
			return elem.GetUint64();
		}
		return val;
	}

	sf::IntRect getIntRectVal(const Value& elem, const sf::IntRect& val)
	{
		if (elem.IsArray() == true
			&& elem.Size() >= 4)
		{
			return sf::IntRect(getIntVal(elem[0]), getIntVal(elem[1]),
				getIntVal(elem[2]), getIntVal(elem[3]));
		}
		else if (elem.IsArray() == true
			&& elem.Size() >= 2)
		{
			return sf::IntRect(0, 0, getIntVal(elem[0]), getIntVal(elem[1]));
		}
		return val;
	}

	sf::FloatRect getFloatRectVal(const Value& elem, const sf::FloatRect& val)
	{
		if (elem.IsArray() == true
			&& elem.Size() >= 4)
		{
			return sf::FloatRect(getFloatVal(elem[0]), getFloatVal(elem[1]),
				getFloatVal(elem[2]), getFloatVal(elem[3]));
		}
		else if (elem.IsArray() == true
			&& elem.Size() >= 2)
		{
			return sf::FloatRect(0, 0, getFloatVal(elem[0]), getFloatVal(elem[1]));
		}
		return val;
	}

	sf::Keyboard::Key getKeyCodeVal(const Value& elem, sf::Keyboard::Key val)
	{
		if (elem.IsInt() == true)
		{
			return GameUtils::getKeyCode(elem.GetInt(), val);
		}
		else if (elem.IsString() == true)
		{
			return GameUtils::getKeyCode(elem.GetString(), val);
		}
		return val;
	}

	size_t getInventoryItemIndexVal(const Value& elem,
		PlayerInventory inv)
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
				itemIdx = (size_t)GameUtils::getPlayerItemMount(elem.GetString());
			}
		}
		return itemIdx;
	}

	InventoryPosition getInventoryPositionVal(const Value& elem, InventoryPosition val)
	{
		if (elem.IsString() == true)
		{
			return GameUtils::getInventoryPosition(elem.GetString(), val);
		}
		return val;
	}

	ItemXY getItemXYVal(const Value& elem, const ItemXY& val)
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
				return ItemXY((uint8_t)x, (uint8_t)y);
			}
		}
		return val;
	}

	ItemCoordInventory getItemCoordInventoryVal(const Value& elem)
	{
		std::string playerId;
		if (elem.HasMember("playerId") == true &&
			elem["playerId"].IsString() == true)
		{
			playerId = elem["playerId"].GetString();
		}
		PlayerInventory inv = PlayerInventory::Body;
		if (elem.HasMember("inventory") == true)
		{
			inv = getPlayerInventoryVal(elem["inventory"]);
			if (elem.HasMember("item") == true)
			{
				if (elem["item"].IsArray() == true)
				{
					auto itemPos = getItemXYVal(elem["item"]);
					return ItemCoordInventory(playerId, (size_t)inv, itemPos);
				}
				else
				{
					size_t itemIdx = getInventoryItemIndexVal(elem["item"], inv);
					return ItemCoordInventory(playerId, (size_t)inv, itemIdx);
				}
			}
			return ItemCoordInventory(playerId, (size_t)inv, 0);
		}
		return ItemCoordInventory(playerId);
	}

	ItemLocation getItemLocationVal(const Value& elem)
	{
		if (elem.HasMember("mapPosition") == true)
		{
			const auto& mapElem = elem["mapPosition"];
			if (mapElem.IsArray() == true
				&& mapElem.Size() > 1
				&& mapElem[0].IsUint() == true
				&& mapElem[1].IsUint() == true)
			{
				return{ MapCoord((uint16_t)mapElem[0].GetUint(), (uint16_t)mapElem[1].GetUint()) };
			}
		}
		return{ getItemCoordInventoryVal(elem) };
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
			return GameUtils::getPlayerInventory(elem.GetString(), val);
		}
		return val;
	}

	Variable getVariableVal(const Value& elem)
	{
		Variable var;
		if (elem.IsString() == true)
		{
			var.set<std::string>(std::string(elem.GetString()));
		}
		else if (elem.IsInt64() == true)
		{
			var.set<int64_t>(elem.GetInt64());
		}
		else if (elem.IsDouble() == true)
		{
			var.set<double>(elem.GetDouble());
		}
		else if (elem.IsBool() == true)
		{
			var.set<bool>(elem.GetBool());
		}
		return var;
	}
}
