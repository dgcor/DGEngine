#include "ParseNamer.h"
#include "Game/Namer.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	NameValue parseNameValue(const Value& elem)
	{
		NameValue value;
		value.min = getMinMaxIntKey<LevelObjValue>(elem, "min",
			std::numeric_limits<LevelObjValue>::min());
		value.max = getMinMaxIntKey<LevelObjValue>(elem, "max",
			std::numeric_limits<LevelObjValue>::max());
		value.text = getStringKey(elem, "text");
		return value;
	}

	bool parseNameValueInterval(const Value& elem, NameValueInterval& value)
	{
		if (isValidString(elem, "property") == false)
		{
			if (elem.HasMember("value") == true)
			{
				const auto& elemValues = elem["value"];
				if (elemValues.IsObject() == true)
				{
					NameValue nameValue;
					nameValue.min = std::numeric_limits<LevelObjValue>::min();
					nameValue.max = std::numeric_limits<LevelObjValue>::max();
					nameValue.text = getStringKey(elemValues, "text");
					value.values.push_back(nameValue);
					return true;
				}
			}
			return false;
		}
		if (elem.HasMember("value") == false)
		{
			return false;
		}
		const auto& elemValues = elem["value"];
		if (elemValues.IsObject() == true)
		{
			NameValue nameValue = parseNameValue(elemValues);
			value.values.push_back(nameValue);
		}
		else if (elemValues.IsArray() == true)
		{
			for (const auto& val : elemValues)
			{
				NameValue nameValue = parseNameValue(val);
				value.values.push_back(nameValue);
			}
		}
		if (value.values.empty() == true)
		{
			return false;
		}
		value.property = elem["property"].GetString();
		return true;
	}

	std::vector<NameValueInterval> parseNameValueIntervals(const Value& elem)
	{
		std::vector<NameValueInterval> values;
		if (elem.IsObject() == true)
		{
			NameValueInterval value;
			if (parseNameValueInterval(elem, value) == true)
			{
				values.push_back(value);
			}
		}
		else if (elem.IsArray() == true)
		{
			for (const auto& val : elem)
			{
				NameValueInterval value;
				if (parseNameValueInterval(val, value) == true)
				{
					values.push_back(value);
				}
			}
		}
		else if (elem.IsString() == true)
		{
			NameValueInterval value;
			NameValue nameValue;
			nameValue.min = std::numeric_limits<LevelObjValue>::min();
			nameValue.max = std::numeric_limits<LevelObjValue>::max();
			nameValue.text = elem.GetString();
			value.values.push_back(nameValue);
			values.push_back(value);
		}
		return values;
	}

	void parseNamer(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		std::string id = elem["id"].GetString();
		if (isValidId(id) == false)
		{
			return;
		}
		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		if (elem.HasMember("names") == false)
		{
			return;
		}
		auto values = parseNameValueIntervals(elem["names"]);
		if (values.empty() == true)
		{
			return;
		}
		level->addNamer(id, std::make_unique<Namer>(std::move(values)));
	}
}
