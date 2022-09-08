#pragma once

#include "Game/Queryable.h"
#include <string>
#include <string_view>

class Quest : public Queryable
{
private:
	std::string id;
	std::string name;
	int state{ 0 };

public:
	Quest() noexcept = default;
	Quest(const std::string_view id_, const std::string_view name_, int state_)
		: id(id_), name(name_), state(state_) {}

	auto& Id() const noexcept { return id; }
	void Id(const std::string_view id_) { id = id_; }
	auto& Name() const noexcept { return name; }
	void Name(const std::string_view name_) { name = name_; }
	auto State() const noexcept { return state; }
	void State(int state_) noexcept { state = state_; }

	bool getProperty(const std::string_view prop, Variable& var) const noexcept override;
};
