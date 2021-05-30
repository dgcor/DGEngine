#include "ParseUtils.h"
#include <cctype>
#include "FileUtils.h"
#include "Json/JsonUtils.h"
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
		id = FileUtils::getFileNameWithoutExt(file);
		if (id.empty() == false && id[0] == '.')
		{
			id.clear();
		}
		return (id.empty() == false);
	}

	bool isValidArray(const Value& elem, const std::string_view key)
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
			if (std::isalnum((unsigned char)ch) != 0 || ch == '_')
			{
				continue;
			}
			return false;
		}
		return true;
	}

	bool isValidString(const Value& elem)
	{
		return (elem.IsString() == true
			&& elem.GetStringLength() > 0);
	}

	bool isValidString(const Value& elem, const std::string_view key)
	{
		return (elem.HasMember(key) == true
			&& elem[key].IsString() == true
			&& elem[key].GetStringLength() > 0);
	}
}
