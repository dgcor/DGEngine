#pragma once

#include <vector>
#include <string>

#include <cstdint>

class Sol
{
private:
	std::vector<uint8_t> data;

public:
	Sol(const std::string& path);

	uint8_t get(size_t index) const;

	bool passable(size_t index) const;

	size_t size() const { return data.size(); }
};
