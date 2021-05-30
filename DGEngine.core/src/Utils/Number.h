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
	constexpr Number32() noexcept : type(0), intNum(0) {}
	constexpr explicit Number32(int32_t num) noexcept : type(0), intNum(num) {}
	constexpr explicit Number32(uint32_t num) noexcept : type(1), uintNum(num) {}
	constexpr explicit Number32(float num) noexcept : type(2), floatNum(num) {}

	constexpr bool isInt32() const noexcept
	{
		return type == 0;
	}
	constexpr bool isUInt32() const noexcept
	{
		return type == 1;
	}
	constexpr bool isFloat() const noexcept
	{
		return type == 2;
	}

	constexpr void setInt32(int32_t num) noexcept
	{
		type = 0;
		intNum = num;
	}
	constexpr void setUInt32(uint32_t num) noexcept
	{
		type = 1;
		uintNum = num;
	}
	constexpr void setFloat(float num) noexcept
	{
		type = 2;
		floatNum = num;
	}

	constexpr int32_t getInt32() const noexcept
	{
		switch (type)
		{
		default:
		case 0:
			return intNum;
		case 1:
			return (int32_t)uintNum;
		case 2:
			return (int32_t)floatNum;
		}
	}
	constexpr uint32_t getUInt32() const noexcept
	{
		switch (type)
		{
		default:
		case 0:
			return (uint32_t)intNum;
		case 1:
			return uintNum;
		case 2:
			return (uint32_t)floatNum;
		}
	}
	constexpr float getFloat() const noexcept
	{
		switch (type)
		{
		default:
		case 0:
			return (float)intNum;
		case 1:
			return (float)uintNum;
		case 2:
			return floatNum;
		}
	}
	constexpr int64_t getInt64() const noexcept
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
	constexpr double getDouble() const noexcept
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

	constexpr Number32& operator+=(const int64_t& right) noexcept
	{
		switch (type)
		{
		default:
		case 0:
			intNum = ((int32_t)((int64_t)intNum + right));
			break;
		case 1:
			uintNum = ((uint32_t)((int64_t)uintNum + right));
			break;
		case 2:
			floatNum = ((float)((double)floatNum + (double)right));
			break;
		}
		return *this;
	}
	constexpr Number32& operator+=(const double& right) noexcept
	{
		switch (type)
		{
		default:
		case 0:
			intNum = ((int32_t)((int64_t)intNum + (int64_t)right));
			break;
		case 1:
			uintNum = ((uint32_t)((int64_t)uintNum + (int64_t)right));
			break;
		case 2:
			floatNum = ((float)((double)floatNum + right));
			break;
		}
		return *this;
	}
	constexpr Number32& operator-=(const int64_t& right) noexcept
	{
		switch (type)
		{
		default:
		case 0:
			intNum = ((int32_t)((int64_t)intNum - right));
			break;
		case 1:
			uintNum = ((uint32_t)((int64_t)uintNum - right));
			break;
		case 2:
			floatNum = ((float)((double)floatNum - (double)right));
			break;
		}
		return *this;
	}
	constexpr Number32& operator-=(const double& right) noexcept
	{
		switch (type)
		{
		default:
		case 0:
			intNum = ((int32_t)((int64_t)intNum - (int64_t)right));
			break;
		case 1:
			uintNum = ((uint32_t)((int64_t)uintNum - (int64_t)right));
			break;
		case 2:
			floatNum = ((float)((double)floatNum - right));
			break;
		}
		return *this;
	}

	constexpr bool operator==(const Number32& right) const noexcept
	{
		return (type == right.type &&
			intNum == right.intNum);
	}
};

constexpr bool operator==(const Number32& left, const int32_t& right) noexcept
{
	return (left.isInt32() == true &&
		left.getInt32() == right);
}

constexpr bool operator==(const Number32& left, const uint32_t& right) noexcept
{
	return (left.isUInt32() == true &&
		left.getUInt32() == right);
}

constexpr bool operator==(const Number32& left, const float& right) noexcept
{
	return (left.isFloat() == true &&
		left.getFloat() == right);
}
