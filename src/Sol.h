#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Sol
{
private:
	std::vector<uint8_t> data;

public:
	Sol() {}
	Sol(const std::string_view fileName);

	uint8_t get(size_t index) const;
	bool passable(size_t index) const;

	bool empty() const noexcept { return data.empty(); }
	size_t size() const noexcept { return data.size(); }
};
