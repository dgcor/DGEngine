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

	bool getBoolVal(const rapidjson::Value& elem, bool val)
	{
		if (elem.IsBool() == true)
		{
			return elem.GetBool();
		}
		else
		{
			return val;
		}
	}

	double getDoubleVal(const rapidjson::Value& elem, double val)
	{
		if (elem.IsDouble() == true)
		{
			return elem.GetDouble();
		}
		else
		{
			return val;
		}
	}

	int getIntVal(const rapidjson::Value& elem, int val)
	{
		if (elem.IsInt() == true)
		{
			return elem.GetInt();
		}
		else
		{
			return val;
		}
	}

	int64_t getInt64Val(const rapidjson::Value& elem, int64_t val)
	{
		if (elem.IsInt64() == true)
		{
			return elem.GetInt64();
		}
		else
		{
			return val;
		}
	}

	const char* getStringCharVal(const rapidjson::Value& elem, const char* val)
	{
		if (elem.IsString() == true)
		{
			return elem.GetString();
		}
		else
		{
			return val;
		}
	}

	std::string getStringVal(const rapidjson::Value& elem, const std::string& val)
	{
		return getStringCharVal(elem, val.c_str());
	}

	unsigned getUIntVal(const rapidjson::Value& elem, unsigned val)
	{
		if (elem.IsUint() == true)
		{
			return elem.GetUint();
		}
		else
		{
			return val;
		}
	}

	uint64_t getUInt64Val(const rapidjson::Value& elem, uint64_t val)
	{
		if (elem.IsUint64() == true)
		{
			return elem.GetUint64();
		}
		else
		{
			return val;
		}
	}

	sf::IntRect getIntRectVal(const rapidjson::Value& elem, const sf::IntRect& val)
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

	sf::FloatRect getFloatRectVal(const rapidjson::Value& elem, const sf::FloatRect& val)
	{
		if (elem.IsArray() == true
			&& elem.Size() >= 4)
		{
			return sf::FloatRect(getDoubleVal(elem[0]), getDoubleVal(elem[1]),
				getDoubleVal(elem[2]), getDoubleVal(elem[3]));
		}
		else if (elem.IsArray() == true
			&& elem.Size() >= 2)
		{
			return sf::FloatRect(0, 0, getDoubleVal(elem[0]), getDoubleVal(elem[1]));
		}
		return val;
	}

	sf::Keyboard::Key getKeyCodeVal(const rapidjson::Value& elem, sf::Keyboard::Key val)
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

	Variable getVariableVal(const rapidjson::Value& elem)
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
