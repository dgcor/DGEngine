#include "ParseUtils.h"
#include <cctype>
#include "FileUtils.h"
#include "Json/JsonUtils.h"
#include <regex>
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	ReplaceVars getReplaceVars(const std::string_view str, ReplaceVars val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("none"):
			return ReplaceVars::None;
		case str2int16("string"):
			return ReplaceVars::String;
		case str2int16("value"):
			return ReplaceVars::Value;
		default:
			return val;
		}
	}

	bool getIdFromFile(const std::string_view file, std::string& id)
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

	bool isValidId(const std::string_view id) noexcept
	{
		if (id.empty() == true)
		{
			return false;
		}
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
}
