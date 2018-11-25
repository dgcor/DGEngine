#pragma once

#include "FileUtils.h"
#include <functional>
#include "JsonParser.h"
#include "Queryable.h"
#include <string_view>
#include "Variable.h"

class Game;

namespace JsonUtils
{
	// replaces "%str%" with obj.getProperty("str")
	void replaceStringWithQueryable(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator,
		const Queryable& obj, bool changeValueType = false);

	void replaceStringWithVariable(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator,
		const std::string_view str, const Variable& var, bool changeValueType = false);

	void replaceValueWithVariable(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator, const Variable& var);

	void replaceValueWithQueryable(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator, const Queryable& obj,
		const std::function<void(rapidjson::Value&)> valFunc = {});

	void replaceValueWithString(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator,
		const std::string_view oldStr, const std::string_view newStr);

	// replaces "%str%" with game.getVarOrProp("str")
	void replaceValueWithGameVar(rapidjson::Value& elem,
	rapidjson::Value::AllocatorType& allocator,
		const Game& obj, bool changeValueType, char token = '%');

	std::string toString(const rapidjson::Value& elem);
	std::string jsonToString(const rapidjson::Value& elem);

	bool loadFile(const std::string_view file, rapidjson::Document& doc);
	bool loadJson(const std::string_view json, rapidjson::Document& doc);

	void saveToFile(const std::string_view file, const rapidjson::Value& elem);

	template <class T>
	void saveToFile(const std::string_view file, const char* key, const T& container)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		writer.SetIndent(' ', 2);
		writer.StartObject();
		writer.Key(key);
		writer.StartObject();
		for (const auto& elem : container)
		{
			writer.String(elem.first);
			const Variable& var = elem.second;
			if (std::holds_alternative<std::string>(var))
			{
				writer.String(std::get<std::string>(var));
			}
			else if (std::holds_alternative<int64_t>(var))
			{
				writer.Int64(std::get<int64_t>(var));
			}
			else if (std::holds_alternative<double>(var))
			{
				writer.Double(std::get<double>(var));
			}
			else if (std::holds_alternative<bool>(var))
			{
				writer.Bool(std::get<bool>(var));
			}
			else
			{
				writer.Null();
			}
		}
		writer.EndObject();
		writer.EndObject();

		FileUtils::saveText(file, { buffer.GetString(), buffer.GetSize() });
	}
}
