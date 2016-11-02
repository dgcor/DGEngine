#pragma once

#include "Json/JsonParser.h"
#include <string>

namespace Parser
{
	bool getBoolIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, bool val = false);

	double getDoubleIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, double val = 0.0);

	int getIntIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, int val = 0);

	int64_t getInt64Idx(const rapidjson::Value& elem, rapidjson::SizeType idx, int64_t val = 0);

	const char* getStringCharIdx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, const char* val = "");

	std::string getStringIdx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, const std::string& val = "");

	unsigned getUIntIdx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, unsigned val = 0);

	uint64_t getUInt64Idx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, uint64_t val = 0);
}
