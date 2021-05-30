#include "SaveUtils.h"

namespace SaveUtils
{
	using namespace rapidjson;

	bool hasProperty(const Save::Properties& properties, const std::string_view key)
	{
		return properties.find(sv2str(key)) != properties.cend();
	}

	bool getBoolProperty(const Save::Properties& properties, const std::string_view key)
	{
		const auto elem = properties.find(sv2str(key));
		if (elem != properties.cend())
		{
			return VarUtils::toBool(elem->second);
		}
		return false;
	}

	int64_t getInt64Property(const Save::Properties& properties, const std::string_view key)
	{
		const auto elem = properties.find(sv2str(key));
		if (elem != properties.cend())
		{
			return VarUtils::toInt64(elem->second);
		}
		return 0;
	}

	double getDoubleProperty(const Save::Properties& properties, const std::string_view key)
	{
		const auto elem = properties.find(sv2str(key));
		if (elem != properties.cend())
		{
			return VarUtils::toDouble(elem->second);
		}
		return 0.0;
	}

	std::string getStringProperty(const Save::Properties& properties, const std::string_view key)
	{
		const auto elem = properties.find(sv2str(key));
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

	void writeKey(PrettyWriter<StringBuffer>& writer,
		const std::string& key)
	{
		writer.Key(key);
	}

	void writeKeyStringView(PrettyWriter<StringBuffer>& writer,
		const std::string_view key)
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

	void writeString(PrettyWriter<StringBuffer>& writer,
		const std::string& val)
	{
		writer.String(val);
	}

	void writeStringView(PrettyWriter<StringBuffer>& writer,
		const std::string_view val)
	{
		writer.String(val.data(), (SizeType)val.size());
	}
}
