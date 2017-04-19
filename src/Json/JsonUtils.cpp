#include "JsonUtils.h"
#include "Game.h"
#include <regex>
#include "Utils.h"

namespace JsonUtils
{
	using namespace rapidjson;

	void replaceValueWithVariable(Value& elem,
		Value::AllocatorType& allocator, const Variable& var)
	{
		if (var.is<std::string>() == true)
		{
			const auto& str = var.get<std::string>();
			elem.SetString(str.c_str(), str.size(), allocator);
		}
		else if (var.is<int64_t>() == true)
		{
			elem.SetInt64(var.get<int64_t>());
		}
		else if (var.is<double>() == true)
		{
			elem.SetDouble(var.get<double>());
		}
		else if (var.is<bool>() == true)
		{
			elem.SetBool(var.get<bool>());
		}
	}

	std::regex regexPercent(R"((\%[\w.]+\%))");

	void replaceStringWithVariable(Value& elem,
		Value::AllocatorType& allocator, const std::string& str,
		const Variable& var, bool changeValueType)
	{
		if (elem.IsString() == false ||
			elem.GetStringLength() == 0)
		{
			return;
		}
		std::string str1(elem.GetString());

		if (changeValueType == true &&
			str1 == str)
		{
			replaceValueWithVariable(elem, allocator, var);
			return;
		}
		Utils::replaceStringInPlace(str1, str, VarUtils::toString(var));

		if (elem.GetString() != str1)
		{
			elem.SetString(str1.c_str(), str1.size(), allocator);
		}
	}

	void replaceStringWithQueryable(Value& elem,
		Value::AllocatorType& allocator,
		const Queryable& obj, bool changeValueType)
	{
		if (elem.IsString() == false ||
			elem.GetStringLength() == 0)
		{
			return;
		}
		std::string str1(elem.GetString());
		std::string str2(str1);
		std::smatch match;
		bool hadMatch = false;
		while (std::regex_search(str1, match, regexPercent) == true)
		{
			hadMatch = true;
			auto strProp = match[1].str();
			Variable var;
			if (obj.getProperty(strProp.substr(1, strProp.size() - 2), var) == true)
			{
				if (changeValueType == true &&
					strProp == str2)
				{
					replaceValueWithVariable(elem, allocator, var);
					return;
				}
				Utils::replaceStringInPlace(
					str2, strProp, VarUtils::toString(var));
			}
			str1 = match.suffix().str();
		}
		if (hadMatch == true &&
			elem.GetString() != str2)
		{
			elem.SetString(str2.c_str(), str2.size(), allocator);
		}
	}

	void replaceValueWithString(Value& elem, Value::AllocatorType& allocator,
		const std::string& oldStr, const std::string& newStr)
	{
		if (elem.IsString() == true)
		{
			std::string str1(elem.GetString());
			auto str2(str1);
			Utils::replaceStringInPlace(str2, oldStr, newStr);
			if (str1 != str2)
			{
				elem.SetString(str2.c_str(), str2.size(), allocator);
			}
		}
		else if (elem.IsObject() == true)
		{
			for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
			{
				replaceValueWithString(it->value, allocator, oldStr, newStr);
			}
		}
		else if (elem.IsArray() == true)
		{
			for (auto it = elem.Begin(); it != elem.End(); ++it)
			{
				replaceValueWithString(*it, allocator, oldStr, newStr);
			}
		}
	}

	void replaceValueWithQueryable(Value& elem,
		Value::AllocatorType& allocator, const Queryable& obj,
		const std::function<void(Value&)> valFunc)
	{
		if (elem.IsString() == true)
		{
			replaceStringWithQueryable(elem, allocator, obj, true);
			if (valFunc)
			{
				valFunc(elem);
			}
		}
		else if (elem.IsObject() == true)
		{
			for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
			{
				replaceValueWithQueryable(it->value, allocator, obj, valFunc);
			}
		}
		else if (elem.IsArray() == true)
		{
			for (auto it = elem.Begin(); it != elem.End(); ++it)
			{
				replaceValueWithQueryable(*it, allocator, obj, valFunc);
			}
		}
	}

	std::regex regexPercent2(R"((\%\w+\%))");

	void replaceValueWithGameVar(Value& elem,
		Value::AllocatorType& allocator,
		const Game& game, bool changeValueType)
	{
		if (elem.IsString() == true)
		{
			std::string str1(elem.GetString());
			std::string str2(str1);
			std::smatch match;
			bool hadMatch = false;
			while (std::regex_search(str1, match, regexPercent2) == true)
			{
				hadMatch = true;
				auto strProp = match[1].str();
				Variable var;
				if (game.getVariable(strProp, var) == true)
				{
					if (changeValueType == true &&
						strProp == str2)
					{
						replaceValueWithVariable(elem, allocator, var);
						return;
					}
					Utils::replaceStringInPlace(
						str2, strProp, VarUtils::toString(var));
				}
				str1 = match.suffix().str();
			}
			if (hadMatch == true &&
				elem.GetString() != str2)
			{
				elem.SetString(str2.c_str(), str2.size(), allocator);
			}
		}
		else if (elem.IsObject() == true)
		{
			for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
			{
				replaceValueWithGameVar(it->value, allocator, game);
			}
		}
		else if (elem.IsArray() == true)
		{
			for (auto it = elem.Begin(); it != elem.End(); ++it)
			{
				replaceValueWithGameVar(*it, allocator, game);
			}
		}
	}

	std::string toString(const Value& elem)
	{
		if (elem.IsString() == true)
		{
			return elem.GetString();
		}
		else if (elem.IsInt64() == true)
		{
			return std::to_string(elem.GetInt64());
		}
		else if (elem.IsDouble() == true)
		{
			return std::to_string(elem.GetDouble());
		}
		else if (elem.IsBool() == true)
		{
			if (elem.GetBool() == true)
			{
				return { "true" };
			}
			else
			{
				return { "false" };
			}
		}
		else if (elem.IsNull() == true)
		{
			return { "null" };
		}
		else if (elem.IsUint64() == true)
		{
			return std::to_string(elem.GetUint64());
		}
		return jsonToString(elem);
	}

	std::string jsonToString(const Value& elem)
	{
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		elem.Accept(writer);
		return std::string(buffer.GetString());
	}

	void saveToFile(const std::string& file, const Value& elem)
	{
		FileUtils::saveText(file.c_str(), jsonToString(elem));
	}
}
