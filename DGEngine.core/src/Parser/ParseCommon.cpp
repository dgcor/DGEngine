#include "ParseCommon.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::string_view parseValidId(const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return {};
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return {};
		}
		return id;
	}

	std::string parseValidIdOrFile(const Value& elem, const std::string_view fileKey)
	{
		std::string id;
		if (isValidString(elem, "id") == true)
		{
			id = elem["id"sv].GetStringView();
		}
		else
		{
			if (isValidString(elem, fileKey) == false)
			{
				return {};
			}
			auto file = getStringViewVal(elem[fileKey]);
			if (getIdFromFile(file, id) == false)
			{
				return {};
			}
		}
		if (isValidId(id) == false)
		{
			return {};
		}
		return id;
	}

	std::string parseValidIdOrFilePath(const Value& elem, const std::string_view filePath)
	{
		std::string id;
		if (isValidString(elem, "id") == true)
		{
			id = elem["id"sv].GetStringView();
		}
		else if (getIdFromFile(filePath, id) == false)
		{
			return {};
		}
		if (isValidId(id) == false)
		{
			return {};
		}
		return id;
	}
}
