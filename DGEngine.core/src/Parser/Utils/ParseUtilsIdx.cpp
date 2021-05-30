#include "ParseUtilsIdx.h"
#include "ParseUtilsVal.h"

namespace Parser
{
	using namespace rapidjson;

	bool getBoolIdx(const Value& elem, SizeType idx, bool val)
	{
		if (idx < elem.Size()
			&& elem[idx].IsBool() == true)
		{
			return elem[idx].GetBool();
		}
		return val;
	}

	double getDoubleIdx(const Value& elem, SizeType idx, double val)
	{
		if (idx < elem.Size()
			&& elem[idx].IsDouble() == true)
		{
			return elem[idx].GetDouble();
		}
		return val;
	}

	int getIntIdx(const Value& elem, SizeType idx, int val)
	{
		if (idx < elem.Size()
			&& elem[idx].IsInt() == true)
		{
			return elem[idx].GetInt();
		}
		return val;
	}

	int64_t getInt64Idx(const Value& elem, SizeType idx, int64_t val)
	{
		if (idx < elem.Size()
			&& elem[idx].IsInt64() == true)
		{
			return elem[idx].GetInt64();
		}
		return val;
	}

	const char* getStringCharIdx(const Value& elem,
		SizeType idx, const char* val)
	{
		if (idx < elem.Size()
			&& elem[idx].IsString() == true)
		{
			return elem[idx].GetString();
		}
		return val;
	}

	std::string getStringIdx(const Value& elem,
		SizeType idx, const std::string& val)
	{
		if (idx < elem.Size())
		{
			return getStringVal(elem[idx], val);
		}
		return val;
	}

	std::string_view getStringViewIdx(const Value& elem,
		SizeType idx, const std::string_view val)
	{
		if (idx < elem.Size())
		{
			return getStringViewVal(elem[idx], val);
		}
		return val;
	}

	unsigned getUIntIdx(const Value& elem, SizeType idx, unsigned val)
	{
		if (idx < elem.Size()
			&& elem[idx].IsUint() == true)
		{
			return elem[idx].GetUint();
		}
		return val;
	}

	uint64_t getUInt64Idx(const Value& elem, SizeType idx, uint64_t val)
	{
		if (idx < elem.Size()
			&& elem[idx].IsUint64() == true)
		{
			return elem[idx].GetUint64();
		}
		return val;
	}
}
