#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Min
{
private:
	std::vector<std::vector<int16_t>> pillars;

public:
	Min(const std::string& filename, size_t minSize);

	const std::vector<int16_t>& operator[] (size_t index) const { return pillars[index]; }
	size_t size() const { return pillars.size(); }
};
