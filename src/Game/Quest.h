#pragma once

#include "Queryable.h"
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

	const std::string& Id() const noexcept { return id; }
	void Id(const std::string_view id_) { id = id_; }
	const std::string& Name() const noexcept { return name; }
	void Name(const std::string_view name_) { name = name_; }
	int State() const noexcept { return state; }
	void State(int state_) noexcept { state = state_; }

	virtual bool getProperty(const std::string_view prop, Variable& var) const noexcept;
};
