#include "ParseUtils.h"
#include <cctype>
#include "FileUtils.h"

namespace Parser
{
	bool getIdFromFile(const std::string& file, std::string& id)
	{
		id = FileUtils::getFileWithoutExt(FileUtils::getFileFromPath(file));
		return (id.empty() == false);
	}

	bool isValidArray(const rapidjson::Value& elem, const char* key)
	{
		return (elem.HasMember(key) && elem[key].IsArray() && elem[key].Size() > 0);
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

	bool isValidString(const rapidjson::Value& elem, const char* key)
	{
		return (elem.HasMember(key) && elem[key].IsString() && elem[key].GetStringLength() > 0);
	}
}
