#pragma once

#include "LevelObjectClass.h"
#include <string_view>
#include <vector>
#include "Utils/Utils.h"

template <class T>
class LevelObjectClassDefaults : public LevelObjectClass
{
protected:
	std::vector<std::pair<uint16_t, T>> defaults;

public:
	const std::vector<std::pair<uint16_t, T>>& Defaults() const noexcept { return defaults; }

	void setDefaultByHash(uint16_t propHash, T val)
	{
		if (propHash == str2int16(""))
		{
			return;
		}
		for (auto& elem : defaults)
		{
			if (elem.first == propHash)
			{
				elem.second = val;
				return;
			}
		}
		defaults.push_back(std::make_pair(propHash, val));
	}

	void setDefault(const std::string_view prop, T val)
	{
		setDefaultByHash(str2int16(prop), val);
	}

	bool isDefault(uint16_t propHash, T& value) const noexcept
	{
		for (const auto& elem : defaults)
		{
			if (elem.first == propHash)
			{
				if (elem.second == value)
				{
					return true;
				}
				break;
			}
		}
		return false;
	}

	bool isDefault(const std::pair<uint16_t, T>& prop) const noexcept
	{
		for (const auto& elem : defaults)
		{
			if (elem.first == prop.first)
			{
				if (elem.second == prop.second)
				{
					return true;
				}
				break;
			}
		}
		return false;
	}

	T getDefaultByHash(uint16_t propHash) const noexcept
	{
		T value{};
		getDefaultByHash(propHash, value);
		return value;
	}

	T getDefault(const std::string_view prop) const noexcept
	{
		T value{};
		getDefault(prop, value);
		return value;
	}

	bool getDefault(const std::string_view prop, T& value) const noexcept
	{
		if (defaults.empty() == false)
		{
			return getDefaultByHash(str2int16(prop), value);
		}
		return false;
	}

	bool getDefaultByHash(uint16_t propHash, T& value) const noexcept
	{
		for (const auto& prop : defaults)
		{
			if (prop.first == propHash)
			{
				value = prop.second;
				return true;
			}
		}
		return false;
	}
};
