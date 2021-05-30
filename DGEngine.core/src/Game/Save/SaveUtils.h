#pragma once

#include <cmath>
#include "Json/JsonParser.h"
#include "SaveProperties.h"
#include <string>
#include <string_view>
#include "Utils/Number.h"
#include "Utils/PairXY.h"

namespace SaveUtils
{
	bool hasProperty(const Save::Properties& properties, const std::string_view key);

	bool getBoolProperty(const Save::Properties& properties, const std::string_view key);

	int64_t getInt64Property(const Save::Properties& properties, const std::string_view key);

	double getDoubleProperty(const Save::Properties& properties, const std::string_view key);

	std::string getStringProperty(const Save::Properties& properties, const std::string_view key);

	void writeBool(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, bool val);

	void writeInt(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, int val);

	void writeInt64(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, int64_t val);

	void writeUInt(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, unsigned val);

	void writeUInt64(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, uint64_t val);

	void writeDouble(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, double val);

	void writeNumber32(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, const Number32& val);

	// same line formatted.
	template <class T, class NumType>
	void writeVector2Number(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, const T& val)
	{
		writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
		writer.Key(key.data(), (rapidjson::SizeType)key.size());
		writer.StartArray();
		if constexpr (std::is_integral<NumType>::value == true)
		{
			if constexpr (sizeof(T) <= 4)
			{
				writer.Int(val.x);
				writer.Int(val.y);
			}
			else
			{
				writer.Int64(val.x);
				writer.Int64(val.y);
			}
		}
		else if constexpr (std::is_floating_point<NumType>::value == true)
		{
			writer.Double(val.x);
			writer.Double(val.y);
		}
		writer.EndArray();
		writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);
	}

	// same line formatted.
	template <class T>
	void writeVector2i(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, const T& val)
	{
		writeVector2Number<T, decltype(val.x)>(writer, key, val);
	}

	// same line formatted.
	template <class T>
	void writeVector2f(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, const T& val)
	{
		writeVector2Number<T, decltype(val.x)>(writer, key, val);
	}

	// writes floats. if the float has no decimal part, writes ints
	// same line formatted.
	template <class T>
	void writeVector2fi(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string_view key, const T& val)
	{
		writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
		writer.Key(key.data(), (rapidjson::SizeType)key.size());
		writer.StartArray();
		if constexpr (std::is_integral<decltype(val.x)>::value == true)
		{
			if constexpr (sizeof(T) <= 4)
			{
				writer.Int(val.x);
				writer.Int(val.y);
			}
			else
			{
				writer.Int64(val.x);
				writer.Int64(val.y);
			}
		}
		else if constexpr (std::is_floating_point<decltype(val.x)>::value == true)
		{
			if constexpr (sizeof(T) <= 4)
			{
				float intpart;
				if (std::modf(val.x, &intpart) == 0.0)
				{
					writer.Int((int32_t)val.x);
				}
				else
				{
					writer.Double(val.x);
				}
				if (std::modf(val.y, &intpart) == 0.0)
				{
					writer.Int((int32_t)val.y);
				}
				else
				{
					writer.Double(val.y);
				}
			}
			else
			{
				double intpart;
				if (std::modf(val.x, &intpart) == 0.0)
				{
					writer.Int64((int64_t)val.x);
				}
				else
				{
					writer.Double(val.x);
				}
				if (std::modf(val.y, &intpart) == 0.0)
				{
					writer.Int64((int64_t)val.y);
				}
				else
				{
					writer.Double(val.y);
				}
			}
		}
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
