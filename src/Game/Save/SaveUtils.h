#pragma once

#include "Game/MapCoord.h"
#include "Game/Number.h"
#include "Json/JsonParser.h"
#include <string>
#include <string_view>

namespace SaveUtils
{
	void writeBool(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, bool val);

	void writeInt(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, int val);

	void writeUInt(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, unsigned val);

	void writeNumber32(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, const Number32& val);

	// same line formatted.
	template <class T>
	void writeVector2d(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, const T& val)
	{
		writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
		writer.Key(key.data(), key.size());
		writer.StartArray();
		writer.Int(val.x);
		writer.Int(val.y);
		writer.EndArray();
		writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);
	}

	// write just the key
	void writeKey(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string& key);

	// write just the key
	void writeKeyStringView(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key);

	void writeString(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, const std::string& val);

	void writeStringView(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, const std::string_view val);

	// write just the value (no key)
	void writeString(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string& val);

	// write just the value (no key)
	void writeStringView(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view val);
}
