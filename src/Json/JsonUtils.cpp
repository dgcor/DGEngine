#include "JsonUtils.h"

namespace JsonUtils
{
	using namespace rapidjson;

	std::string toString(const rapidjson::Value& elem)
	{
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		elem.Accept(writer);
		return std::string(buffer.GetString());
	}

	void saveToFile(const std::string& file, const rapidjson::Value& elem)
	{
		FileUtils::saveText(file.c_str(), toString(elem));
	}
}
