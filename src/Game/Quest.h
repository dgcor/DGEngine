#pragma once

#include <string>
#include "Variable.h"

class Quest
{
private:
	std::string id;
	std::string name;
	int state{ 0 };

public:
	Quest() {}
	Quest(const std::string& id_, const std::string& name_, int state_)
		: id(id_), name(name_), state(state_) {}

	const std::string& Id() const { return id; }
	void Id(const std::string& id_) { id = id_; }
	const std::string& Name() const { return name; }
	void Name(const std::string& name_) { name = name_; }
	int State() const { return state; }
	void State(int state_) { state = state_; }

	bool getProperty(const std::string& prop, Variable& var) const;
};
