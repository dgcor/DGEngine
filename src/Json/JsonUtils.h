#pragma once

#include "FileUtils.h"
#include "JsonParser.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <string>
#include "Variable.h"

namespace JsonUtils
{
	std::string toString(const rapidjson::Value& elem);
	void saveToFile(const std::string& file, const rapidjson::Value& elem);

	template <class T>
	void saveToFile(const std::string& file, const char* key, const T& container)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		writer.SetIndent(' ', 2);
		writer.StartObject();
		writer.Key(key);
		writer.StartObject();
		for (const auto& elem : container)
		{
			writer.String(elem.first.c_str());
			const Variable& var = elem.second;
			if (var.is<std::string>())
			{
				writer.String(var.get<std::string>().c_str());
			}
			else if (var.is<int64_t>())
			{
				writer.Int64(var.get<int64_t>());
			}
			else if (var.is<double>())
			{
				writer.Double(var.get<double>());
			}
			else if (var.is<bool>())
			{
				writer.Bool(var.get<bool>());
			}
			else
			{
				writer.Null();
			}
		}
		writer.EndObject();
		writer.EndObject();

		FileUtils::saveText(file.c_str(), buffer.GetString(), buffer.GetSize());
	}
}
