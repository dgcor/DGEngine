#include "SaveUtils.h"
#include "Game/Utils/VarUtils.h"

namespace SaveUtils
{
	using namespace rapidjson;

	bool hasProperty(const UnorderedStringMap<Variable>& properties, const std::string_view key)
	{
		return properties.find(key) != properties.cend();
	}

	bool getBoolProperty(const UnorderedStringMap<Variable>& properties, const std::string_view key)
	{
		const auto elem = properties.find(key);
		if (elem != properties.cend())
		{
			return VarUtils::toBool(elem->second);
		}
		return false;
	}

	int64_t getInt64Property(const UnorderedStringMap<Variable>& properties, const std::string_view key)
	{
		const auto elem = properties.find(key);
		if (elem != properties.cend())
		{
			return VarUtils::toInt64(elem->second);
		}
		return 0;
	}

	double getDoubleProperty(const UnorderedStringMap<Variable>& properties, const std::string_view key)
	{
		const auto elem = properties.find(key);
		if (elem != properties.cend())
		{
			return VarUtils::toDouble(elem->second);
		}
		return 0.0;
	}

	std::string getStringProperty(const UnorderedStringMap<Variable>& properties, const std::string_view key)
	{
		const auto elem = properties.find(key);
		if (elem != properties.cend())
		{
			return VarUtils::toString(elem->second);
		}
		return {};
	}

	void writeBool(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, bool val)
	{
		writer.Key(key.data(), (SizeType)key.size());
		writer.Bool(val);
	}

	void writeInt(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, int val)
	{
		writer.Key(key.data(), (SizeType)key.size());
		writer.Int(val);
	}

	void writeInt64(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, int64_t val)
	{
		writer.Key(key.data(), (SizeType)key.size());
		writer.Int64(val);
	}

	void writeUInt(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, unsigned val)
	{
		writer.Key(key.data(), (SizeType)key.size());
		writer.Uint(val);
	}

	void writeUInt64(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, uint64_t val)
	{
		writer.Key(key.data(), (SizeType)key.size());
		writer.Uint64(val);
	}

	void writeDouble(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, double val)
	{
		writer.Key(key.data(), (SizeType)key.size());
		writer.Double(val);
	}

	void writeNumber32(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, const Number32& val)
	{
		writer.Key(key.data(), (SizeType)key.size());
		if (val.isInt32() == true)
		{
			writer.Int(val.getInt32());
		}
		else if (val.isUInt32() == true)
		{
			writer.Uint(val.getUInt32());
		}
		else
		{
			writer.Double(val.getFloat());
		}
	}

	void writeKey(PrettyWriter<StringBuffer>& writer, const std::string& key)
	{
		writer.Key(key);
	}

	void writeKeyStringView(PrettyWriter<StringBuffer>& writer, const std::string_view key)
	{
		writer.Key(key.data(), (SizeType)key.size());
	}

	void writeString(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, const std::string& val)
	{
		writer.Key(key.data(), (SizeType)key.size());
		writer.String(val);
	}

	void writeStringView(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, const std::string_view val)
	{
		writer.Key(key.data(), (SizeType)key.size());
		writer.String(val.data(), (SizeType)val.size());
	}

	void writeString(PrettyWriter<StringBuffer>& writer, const std::string& val)
	{
		writer.String(val);
	}

	void writeStringView(PrettyWriter<StringBuffer>& writer, const std::string_view val)
	{
		writer.String(val.data(), (SizeType)val.size());
	}

	void writeVariable(PrettyWriter<StringBuffer>& writer, const Variable& val)
	{
		if (std::holds_alternative<std::string>(val))
		{
			writer.String(std::get<std::string>(val));
		}
		else if (std::holds_alternative<int64_t>(val))
		{
			writer.Int64(std::get<int64_t>(val));
		}
		else if (std::holds_alternative<double>(val))
		{
			writer.Double(std::get<double>(val));
		}
		else if (std::holds_alternative<bool>(val))
		{
			writer.Bool(std::get<bool>(val));
		}
		else if (std::holds_alternative<Int64Tuple2>(val))
		{
			writer.StartArray();
			writer.Int64(std::get<0>(std::get<Int64Tuple2>(val)));
			writer.Int64(std::get<1>(std::get<Int64Tuple2>(val)));
			writer.EndArray();
		}
		else if (std::holds_alternative<Int64Tuple3>(val))
		{
			writer.StartArray();
			writer.Int64(std::get<0>(std::get<Int64Tuple3>(val)));
			writer.Int64(std::get<1>(std::get<Int64Tuple3>(val)));
			writer.Int64(std::get<2>(std::get<Int64Tuple3>(val)));
			writer.EndArray();
		}
		else if (std::holds_alternative<Int64Tuple4>(val))
		{
			writer.StartArray();
			writer.Int64(std::get<0>(std::get<Int64Tuple4>(val)));
			writer.Int64(std::get<1>(std::get<Int64Tuple4>(val)));
			writer.Int64(std::get<2>(std::get<Int64Tuple4>(val)));
			writer.Int64(std::get<3>(std::get<Int64Tuple4>(val)));
			writer.EndArray();
		}
		else if (std::holds_alternative<DoubleTuple2>(val))
		{
			writer.StartArray();
			writer.Double(std::get<0>(std::get<DoubleTuple2>(val)));
			writer.Double(std::get<1>(std::get<DoubleTuple2>(val)));
			writer.EndArray();
		}
		else if (std::holds_alternative<DoubleTuple3>(val))
		{
			writer.StartArray();
			writer.Double(std::get<0>(std::get<DoubleTuple3>(val)));
			writer.Double(std::get<1>(std::get<DoubleTuple3>(val)));
			writer.Double(std::get<2>(std::get<DoubleTuple3>(val)));
			writer.EndArray();
		}
		else if (std::holds_alternative<DoubleTuple4>(val))
		{
			writer.StartArray();
			writer.Double(std::get<0>(std::get<DoubleTuple4>(val)));
			writer.Double(std::get<1>(std::get<DoubleTuple4>(val)));
			writer.Double(std::get<2>(std::get<DoubleTuple4>(val)));
			writer.Double(std::get<3>(std::get<DoubleTuple4>(val)));
			writer.EndArray();
		}
		else
		{
			writer.Null();
		}
	}
}
