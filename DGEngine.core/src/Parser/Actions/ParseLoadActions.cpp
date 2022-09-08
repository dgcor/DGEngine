#include "ParseLoadActions.h"
#include "Game/Actions/ActLoad.h"
#include "Game/Utils/FileUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseLoad(const Value& elem)
	{
		return std::make_shared<ActLoad>(getStringVectorKey(elem, "file"));
	}

	std::shared_ptr<Action> parseLoadJson(const Value& elem)
	{
		std::string json;
		if (elem.HasMember("json"sv) == true)
		{
			if (elem["json"sv].IsString())
			{
				json = getStringVal(elem["json"sv]);
			}
			else
			{
				json = JsonUtils::jsonToString(elem["json"sv]);
			}
		}
		else if (isValidString(elem, "file") == true)
		{
			json = FileUtils::readText(elem["file"sv].GetString());
		}
		if (isValidArray(elem, "args") == true)
		{
			return std::make_shared<ActLoadJson>(
				json,
				getStringVectorKey(elem, "args"));
		}
		else
		{
			return std::make_shared<ActLoadJson>(json);
		}
	}

	std::shared_ptr<Action> parseLoadRandom(const Value& elem)
	{
		return std::make_shared<ActLoadRandom>(
			getStringVectorKey(elem, "file"),
			getStringViewKey(elem, "endsWith", ".json"));
	}
}
