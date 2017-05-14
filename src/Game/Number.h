#pragma once

#include <cstdint>

struct Number32
{
private:
	uint8_t type;
	union
	{
		int32_t intNum;
		uint32_t uintNum;
		float floatNum;
	};

public:
	constexpr Number32() : type(0), intNum(0) {}
	constexpr explicit Number32(int32_t num) : type(0), intNum(num) {}
	constexpr explicit Number32(uint32_t num) : type(1), uintNum(num) {}
	constexpr explicit Number32(float num) : type(2), floatNum(num) {}

	constexpr void setInt32(int32_t num)
	{
		type = 0;
		intNum = num;
	}
	constexpr void setUInt32(uint32_t num)
	{
		type = 1;
		uintNum = num;
	}
	constexpr void setFloat(float num)
	{
		type = 2;
		floatNum = num;
	}

	constexpr int32_t getInt32() { return intNum; }
	constexpr uint32_t getUInt32() { return uintNum; }
	constexpr float getFloat() { return floatNum; }

	constexpr int64_t getInt64()
	{
		switch (type)
		{
		default:
		case 0:
			return (int64_t)intNum;
		case 1:
			return (int64_t)uintNum;
		case 2:
			return (int64_t)floatNum;
		}
	}

	constexpr double getDouble()
	{
		switch (type)
		{
		default:
		case 0:
			return (double)intNum;
		case 1:
			return (double)uintNum;
		case 2:
			return (double)floatNum;
		}
	}
};
