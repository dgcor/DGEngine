#pragma once

#include "Utils/UnorderedStringMap.h"
#include "Variable.h"
#include <vector>

class Queryable;

class VariableManager
{
private:
	UnorderedStringMap<Variable> variables;

public:
	auto& getVariables() const noexcept { return variables; }

	// gets variable without tokens. ex: "var"
	bool getVariableNoToken(const std::string_view key, Variable& var) const;

	template <class T, class U>
	U getVarOrProp(const Queryable& queryable, const Variable& var, U defVal = U())
	{
		if (std::holds_alternative<T>(var) == true)
		{
			return (U)std::get<T>(var);
		}
		else if (std::holds_alternative<std::string>(var) == true)
		{
			Variable var2;
			if (getVarOrProp(queryable, std::get<std::string>(var), var2) == true)
			{
				if (std::holds_alternative<T>(var2) == true)
				{
					return (U)std::get<T>(var2);
				}
			}
		}
		return defVal;
	}

	bool getVarOrPropNoToken(const Queryable& queryable, const std::string_view key, Variable& var) const;
	bool getVarOrProp(const Queryable& queryable, const std::string_view key, Variable& var) const;
	Variable getVarOrProp(const Queryable& queryable, const Variable& var) const;
	bool getVarOrPropBoolS(const Queryable& queryable, const std::string_view key) const;
	bool getVarOrPropBoolV(const Queryable& queryable, const Variable& var) const;
	double getVarOrPropDoubleS(const Queryable& queryable, const std::string_view key) const;
	double getVarOrPropDoubleV(const Queryable& queryable, const Variable& var) const;
	int64_t getVarOrPropInt64S(const Queryable& queryable, const std::string_view key) const;
	int64_t getVarOrPropInt64V(const Queryable& queryable, const Variable& var) const;
	bool getVarOrPropStringS(const Queryable& queryable, const std::string_view key, std::string& outStr) const;
	std::string getVarOrPropStringS(const Queryable& queryable, const std::string_view key) const;
	std::string getVarOrPropStringV(const Queryable& queryable, const Variable& var) const;

	// no tokens in key.
	void clearVariable(const std::string_view key);

	// no tokens in key.
	void setVariable(const std::string& key, const Variable& value);

	void saveVariables(const std::string& filePath, const std::vector<std::string>& varNames) const;
};
