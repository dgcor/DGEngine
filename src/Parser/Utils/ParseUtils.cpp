#include "ParseUtils.h"
#include <cctype>
#include "FileUtils.h"
#include "GameUtils.h"
#include <regex>

namespace Parser
{
	using namespace rapidjson;

	bool getIdFromFile(const std::string& file, std::string& id)
	{
		id = FileUtils::getFileWithoutExt(FileUtils::getFileFromPath(file));
		return (id.empty() == false);
	}

	bool isValidArray(const Value& elem, const char* key)
	{
		return (elem.HasMember(key) == true
			&& elem[key].IsArray() == true
			&& elem[key].Size() > 0);
	}

	bool isValidId(const std::string& id)
	{
		for (auto ch : id)
		{
			if (std::isalnum(ch) != 0 || ch == '_')
			{
				continue;
			}
			return false;
		}
		return true;
	}

	bool isValidString(const Value& elem, const char* key)
	{
		return (elem.HasMember(key) == true
			&& elem[key].IsString() == true
			&& elem[key].GetStringLength() > 0);
	}

	void replaceValWithString(Value& value, Value::AllocatorType& allocator,
		const std::string& oldStr, const std::string& newStr)
	{
		if (value.IsObject() == true)
		{
			for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it)
			{
				if (it->value.IsString() == true)
				{
					std::string str1(it->value.GetString());
					auto str2(str1);
					Utils::replaceStringInPlace(str2, oldStr, newStr);
					if (str1 != str2)
					{
						it->value.SetString(str2.c_str(), str2.size(), allocator);
					}
				}
				else
				{
					replaceValWithString(it->value, allocator, oldStr, newStr);
				}
			}
		}
		else if (value.IsArray() == true)
		{
			for (auto it = value.Begin(); it != value.End(); ++it)
			{
				replaceValWithString(*it, allocator, oldStr, newStr);
			}
		}
	}

	void replaceValWithQueryable(Value& value,
		Value::AllocatorType& allocator, const Queryable& obj)
	{
		if (value.IsObject() == true)
		{
			for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it)
			{
				if (it->value.IsString() == true)
				{
					std::string str1(it->value.GetString());
					auto str2 = GameUtils::replaceStringWithQueryable(str1, obj);
					if (str1 != str2)
					{
						it->value.SetString(str2.c_str(), str2.size(), allocator);
					}
				}
				else
				{
					replaceValWithQueryable(it->value, allocator, obj);
				}
			}
		}
		else if (value.IsArray() == true)
		{
			for (auto it = value.Begin(); it != value.End(); ++it)
			{
				replaceValWithQueryable(*it, allocator, obj);
			}
		}
	}

	std::regex regexPercent(R"((\%\w+\%))");

	void replaceValWithGameVar(Value& value,
		Value::AllocatorType& allocator, const Game& game)
	{
		if (value.IsObject() == true)
		{
			for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it)
			{
				if (it->value.IsString() == true)
				{
					std::string str1(it->value.GetString());
					std::string str2(str1);
					std::smatch match;
					while (std::regex_search(str1, match, regexPercent) == true)
					{
						auto strProp = match[1].str();
						Variable var;
						if (game.getVariable(strProp, var) == true)
						{
							Utils::replaceStringInPlace(
								str2, strProp, VarUtils::toString(var));
						}
						str1 = match.suffix().str();
					}
					if (it->value.GetString() != str2)
					{
						it->value.SetString(str2.c_str(), str2.size(), allocator);
					}
				}
				else
				{
					replaceValWithGameVar(it->value, allocator, game);
				}
			}
		}
		else if (value.IsArray() == true)
		{
			for (auto it = value.Begin(); it != value.End(); ++it)
			{
				replaceValWithGameVar(*it, allocator, game);
			}
		}
	}
}
