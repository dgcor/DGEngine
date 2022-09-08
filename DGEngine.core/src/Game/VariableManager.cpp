#include "VariableManager.h"
#include "Game/Queryable.h"
#include "Game/Utils/VarUtils.h"
#include "Json/JsonUtils.h"

bool VariableManager::getVariableNoToken(const std::string_view key, Variable& var) const
{
	auto it = variables.find(key);
	if (it != variables.cend())
	{
		var = it->second;
		return true;
	}
	return false;
}

bool VariableManager::getVarOrPropNoToken(const Queryable& queryable, const std::string_view key, Variable& var) const
{
	if (getVariableNoToken(key, var) == true)
	{
		return true;
	}
	return queryable.getProperty(key, var);
}

bool VariableManager::getVarOrProp(const Queryable& queryable, const std::string_view key, Variable& var) const
{
	if ((key.size() > 2) &&
		(key.front() == '%') &&
		(key.back() == '%'))
	{
		auto key2 = key.substr(1, key.size() - 2);
		return getVarOrPropNoToken(queryable, key2, var);
	}
	return false;
}

Variable VariableManager::getVarOrProp(const Queryable& queryable, const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		Variable var2;
		if (getVarOrProp(queryable, std::get<std::string>(var), var2) == true)
		{
			return var2;
		}
	}
	return var;
}

bool VariableManager::getVarOrPropBoolS(const Queryable& queryable, const std::string_view key) const
{
	Variable var;
	if (getVarOrProp(queryable, key, var) == true)
	{
		return VarUtils::toBool(var);
	}
	return false;
}

bool VariableManager::getVarOrPropBoolV(const Queryable& queryable, const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		return getVarOrPropBoolS(queryable, std::get<std::string>(var));
	}
	return VarUtils::toBool(var);
}

double VariableManager::getVarOrPropDoubleS(const Queryable& queryable, const std::string_view key) const
{
	Variable var;
	if (getVarOrProp(queryable, key, var) == true)
	{
		return VarUtils::toDouble(var);
	}
	return 0.0;
}

double VariableManager::getVarOrPropDoubleV(const Queryable& queryable, const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		return getVarOrPropDoubleS(queryable, std::get<std::string>(var));
	}
	return VarUtils::toDouble(var);
}

int64_t VariableManager::getVarOrPropInt64S(const Queryable& queryable, const std::string_view key) const
{
	Variable var;
	if (getVarOrProp(queryable, key, var) == true)
	{
		return VarUtils::toInt64(var);
	}
	return 0;
}

int64_t VariableManager::getVarOrPropInt64V(const Queryable& queryable, const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		return getVarOrPropInt64S(queryable, std::get<std::string>(var));
	}
	return VarUtils::toInt64(var);
}

bool VariableManager::getVarOrPropStringS(const Queryable& queryable, const std::string_view key, std::string& outStr) const
{
	if ((key.size() > 2) &&
		(key.front() == '%') &&
		(key.back() == '%'))
	{
		auto key2 = key.substr(1, key.size() - 2);
		Variable var;
		if (getVariableNoToken(key2, var) == true)
		{
			if (std::holds_alternative<std::string>(var) == true)
			{
				outStr = std::get<std::string>(var);
				if (queryable.getProperty(outStr, var) == true)
				{
					outStr = VarUtils::toString(var);
				}
			}
			else
			{
				outStr = VarUtils::toString(var);
			}
			return true;
		}
		else if (queryable.getProperty(key2, var) == true)
		{
			outStr = VarUtils::toString(var);
			return true;
		}
	}
	outStr = key;
	return false;
}

std::string VariableManager::getVarOrPropStringS(const Queryable& queryable, const std::string_view key) const
{
	std::string str;
	getVarOrPropStringS(queryable, key, str);
	return str;
}

std::string VariableManager::getVarOrPropStringV(const Queryable& queryable, const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		return getVarOrPropStringS(queryable, std::get<std::string>(var));
	}
	return VarUtils::toString(var);
}

void VariableManager::clearVariable(const std::string_view key)
{
	auto it = variables.find(key);
	if (it != variables.end())
	{
		variables.erase(it);
	}
}

void VariableManager::setVariable(const std::string& key, const Variable& value)
{
	variables[key] = value;
}

void VariableManager::saveVariables(const std::string& filePath, const std::vector<std::string>& varNames) const
{
	if (varNames.empty() == true)
	{
		return;
	}
	std::vector<std::pair<std::string, Variable>> variablesToSave;
	for (const auto& name : varNames)
	{
		auto it = variables.find(name);
		if (it != variables.end())
		{
			variablesToSave.push_back(*it);
		}
	}
	if (variablesToSave.empty() == false)
	{
		JsonUtils::saveToFile(filePath, "variable", variablesToSave);
	}
}
