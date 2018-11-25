#include "SaveUtils.h"

namespace SaveUtils
{
	using namespace rapidjson;

	void writeBool(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, bool val)
	{
		writer.Key(key.data(), key.size());
		writer.Bool(val);
	}

	void writeInt(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, int val)
	{
		writer.Key(key.data(), key.size());
		writer.Int(val);
	}

	void writeUInt(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, unsigned val)
	{
		writer.Key(key.data(), key.size());
		writer.Uint(val);
	}

	void writeNumber32(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, const Number32& val)
	{
		writer.Key(key.data(), key.size());
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
		writer.Key(key.data(), key.size());
	}

	void writeString(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, const std::string& val)
	{
		writer.Key(key.data(), key.size());
		writer.String(val);
	}

	void writeStringView(PrettyWriter<StringBuffer>& writer,
		const std::string_view key, const std::string_view val)
	{
		writer.Key(key.data(), key.size());
		writer.String(val.data(), val.size());
	}

	void writeString(PrettyWriter<StringBuffer>& writer,
		const std::string& val)
	{
		writer.String(val);
	}

	void writeStringView(PrettyWriter<StringBuffer>& writer,
		const std::string_view val)
	{
		writer.String(val.data(), val.size());
	}
}
