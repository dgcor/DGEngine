#include "ParseUtilsIdx.h"

namespace Parser
{
	bool getBoolIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, bool val)
	{
		if (idx < elem.Size() && elem[idx].IsBool()) {
			return elem[idx].GetBool();
		}
		else {
			return val;
		}
	}

	double getDoubleIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, double val)
	{
		if (idx < elem.Size() && elem[idx].IsDouble()) {
			return elem[idx].GetDouble();
		}
		else {
			return val;
		}
	}

	int getIntIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, int val)
	{
		if (idx < elem.Size() && elem[idx].IsInt()) {
			return elem[idx].GetInt();
		}
		else {
			return val;
		}
	}

	int64_t getInt64Idx(const rapidjson::Value& elem, rapidjson::SizeType idx, int64_t val)
	{
		if (idx < elem.Size() && elem[idx].IsInt64()) {
			return elem[idx].GetInt64();
		}
		else {
			return val;
		}
	}

	const char* getStringCharIdx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, const char* val)
	{
		if (idx < elem.Size() && elem[idx].IsString()) {
			return elem[idx].GetString();
		}
		else {
			return val;
		}
	}

	std::string getStringIdx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, const std::string& val)
	{
		return getStringCharIdx(elem, idx, val.c_str());
	}

	unsigned getUIntIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, unsigned val)
	{
		if (idx < elem.Size() && elem[idx].IsUint()) {
			return elem[idx].GetUint();
		}
		else {
			return val;
		}
	}

	uint64_t getUInt64Idx(const rapidjson::Value& elem, rapidjson::SizeType idx, uint64_t val)
	{
		if (idx < elem.Size() && elem[idx].IsUint64()) {
			return elem[idx].GetUint64();
		}
		else {
			return val;
		}
	}
}
