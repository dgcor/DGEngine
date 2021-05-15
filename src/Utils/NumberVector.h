#pragma once

#include "endian/little_endian.hpp"
#include "FileUtils.h"
#include "SFML/PhysFSStream.h"
#include <string_view>
#include "Utils.h"
#include <vector>

// loads list of numbers from file.
template<class T>
class NumberVector
{
protected:
	std::vector<T> data;

public:
	NumberVector() {}

	// calls load
	NumberVector(const std::string_view fileName, T defaultVal = {},
		size_t maxNumElements = std::numeric_limits<size_t>::max())
	{
		load(fileName, defaultVal, maxNumElements);
	}

	// loads fileName as a text file if the file extension is .txt
	// or as a little endian binary file otherwise.
	void load(const std::string_view fileName, T defaultVal = {},
		size_t maxNumElements = std::numeric_limits<size_t>::max())
	{
		if (Utils::endsWith(fileName, ".txt"))
		{
			loadText(fileName, defaultVal, maxNumElements);
		}
		else
		{
			loadBinary(fileName, maxNumElements);
		}
	}

	// loads a list of numbers from a little endian binary file.
	void loadBinary(const std::string_view fileName,
		size_t maxNumElements = std::numeric_limits<size_t>::max())
	{
		sf::PhysFSStream file(fileName.data());
		if (file.hasError() == true)
		{
			return;
		}
		auto size = (size_t)file.getSize();
		if (size < sizeof(T))
		{
			return;
		}
		auto numElements = std::min(size / sizeof(T), maxNumElements);
		data.resize(numElements);
		file.read(data.data(), numElements * sizeof(T));
		if constexpr (sizeof(T) > 1)
		{
			for (auto& val : data)
			{
				val = endian::little_endian::get<T>((const uint8_t*)&val);
			}
		}
	}

	// loads a list of numbers from a new line (\n) separated text file.
	void loadText(const std::string_view fileName, T defaultVal = {},
		size_t maxNumElements = std::numeric_limits<size_t>::max())
	{
		data.clear();
		auto text = FileUtils::readText(fileName.data());
		bool loop = true;
		size_t start = 0;
		size_t pos = 0;
		do
		{
			pos = text.find('\n', start);
			if (pos == std::string::npos)
			{
				pos = text.size();
				if (start == pos)
				{
					return;
				}
				loop = false;
			}
			std::string_view numberStr(text.data() + start, pos - start);
			if (numberStr.empty() == true)
			{
				data.push_back(defaultVal);
			}
			else
			{
				data.push_back(Utils::strtonumber<T>(numberStr));
			}
			if (data.size() >= maxNumElements)
			{
				return;
			}
			start = pos + 1;
		} while (loop == true);
	}

	T get(size_t index) const
	{
		if (index >= data.size())
		{
			return {};
		}
		else
		{
			return data[index];
		}
	}

	T& operator[] (size_t index) { return data[index]; }

	const std::vector<T>& getContainer() const noexcept { return data; }
	bool empty() const noexcept { return data.empty(); }
	size_t size() const noexcept { return data.size(); }
};
