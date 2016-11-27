#include "ParseUtilsKey.h"
#include <cctype>
#include "FileUtils.h"
#include "Game.h"
#include "GameUtils.h"
#include "SFMLUtils.h"
#include "Utils.h"

namespace Parser
{
	using namespace Utils;

	Anchor getAnchorKey(const rapidjson::Value& elem, const char* key, Anchor val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getAnchor(keyElem.GetString(), val);
			}
			else if (keyElem.IsArray() == true)
			{
				Anchor ret = Anchor::None;
				for (const auto& arrElem : keyElem)
				{
					ret |= GameUtils::getAnchor(getStringVal(arrElem).c_str(), val);
				}
				return ret;
			}
		}
		return val;
	}

	bool getBoolKey(const rapidjson::Value& elem, const char* key, bool val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsBool() == true)
		{
			return elem[key].GetBool();
		}
		else
		{
			return val;
		}
	}

	double getDoubleKey(const rapidjson::Value& elem, const char* key, double val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsDouble() == true)
		{
			return elem[key].GetDouble();
		}
		else
		{
			return val;
		}
	}

	int getIntKey(const rapidjson::Value& elem, const char* key, int val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsInt() == true)
		{
			return elem[key].GetInt();
		}
		else
		{
			return val;
		}
	}

	int64_t getInt64Key(const rapidjson::Value& elem, const char* key, int64_t val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsInt64() == true)
		{
			return elem[key].GetInt64();
		}
		else
		{
			return val;
		}
	}

	const char* getStringCharKey(const rapidjson::Value& elem,
		const char* key, const char* val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsString() == true)
		{
			return elem[key].GetString();
		}
		else
		{
			return val;
		}
	}

	std::string getStringKey(const rapidjson::Value& elem,
		const char* key, const std::string& val)
	{
		return getStringCharKey(elem, key, val.c_str());
	}

	unsigned getUIntKey(const rapidjson::Value& elem, const char* key, unsigned val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsUint() == true)
		{
			return elem[key].GetUint();
		}
		else
		{
			return val;
		}
	}

	uint64_t getUInt64Key(const rapidjson::Value& elem, const char* key, uint64_t val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsUint64() == true)
		{
			return elem[key].GetUint64();
		}
		else
		{
			return val;
		}
	}

	sf::IntRect getIntRectKey(const rapidjson::Value& elem,
		const char* key, const sf::IntRect& val)
	{
		if (elem.HasMember(key) == true)
		{
			return getIntRectVal(elem[key], val);
		}
		return val;
	}

	sf::FloatRect getFloatRectKey(const rapidjson::Value& elem,
		const char* key, const sf::FloatRect& val)
	{
		if (elem.HasMember(key) == true)
		{
			return getFloatRectVal(elem[key], val);
		}
		return val;
	}

	sf::Color getColorKey(const rapidjson::Value& elem, const char* key, const sf::Color& val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsString() == true)
		{
			try
			{
				return sf::stringToColor(elem[key].GetString());
			}
			catch (std::exception ex) {}
		}
		return val;
	}

	std::vector<std::string> getStringVectorKey(const rapidjson::Value& elem, const char* key)
	{
		std::vector<std::string> vec;
		if (elem.HasMember(key) == true)
		{
			const auto& elemVal = elem[key];
			if (elemVal.IsArray() == true)
			{
				for (const auto& val : elemVal)
				{
					vec.push_back(val.GetString());
				}
			}
			else if (elemVal.IsString() == true)
			{
				vec.push_back(elemVal.GetString());
			}
		}
		return vec;
	}

	IgnoreResource getIgnoreResourceKey(const rapidjson::Value& elem,
		const char* key, IgnoreResource val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsBool() == true)
			{
				if (keyElem.GetBool() == true)
				{
					return IgnoreResource::DrawAndUpdate;
				}
				else
				{
					return IgnoreResource::None;
				}
			}
			else if (keyElem.IsString() == true)
			{
				return GameUtils::getIgnoreResource(keyElem.GetString(), val);
			}
		}
		return val;
	}

	size_t getInventoryItemIndexKey(const rapidjson::Value& elem,
		const char* key, PlayerInventory inv)
	{
		size_t itemIdx = 0;
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsUint() == true)
			{
				itemIdx = keyElem.GetUint();
			}
			else if (keyElem.IsString() == true)
			{
				if (inv == PlayerInventory::Body)
				{
					itemIdx = (size_t)GameUtils::getPlayerItemMount(keyElem.GetString());
				}
			}
		}
		return itemIdx;
	}

	PlayerDirection getPlayerDirectionKey(const rapidjson::Value& elem,
		const char* key, PlayerDirection val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getPlayerDirection(keyElem.GetString(), val);
			}
		}
		return val;
	}

	PlayerInventory getPlayerInventoryKey(const rapidjson::Value& elem,
		const char* key, PlayerInventory val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getPlayerInventory(keyElem.GetString(), val);
			}
		}
		return val;
	}

	PlayerItemMount getPlayerItemMountKey(const rapidjson::Value& elem,
		const char* key, PlayerItemMount val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getPlayerItemMount(keyElem.GetString(), val);
			}
		}
		return val;
	}

	PlayerStatus getPlayerStatusKey(const rapidjson::Value& elem,
		const char* key, PlayerStatus val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getPlayerStatus(keyElem.GetString(), val);
			}
		}
		return val;
	}

	Variable getVariableKey(const rapidjson::Value& elem, const char* key)
	{
		if (elem.HasMember(key) == true)
		{
			return getVariableVal(elem[key]);
		}
		return Variable();
	}
}
