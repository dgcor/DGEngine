#pragma once

#include "Action.h"
#include "Game.h"
#include "UIObject.h"

class ActVariableAdd : public Action
{
private:
	std::string key;
	Variable value;

public:
	ActVariableAdd(const std::string_view key_, const Variable& value_)
		: key(key_), value(value_) {}

	bool execute(Game& game) override
	{
		Variable value2;
		if (game.getVariableNoToken(key, value2) == true)
		{
			if (std::holds_alternative<int64_t>(value2) == true &&
				std::holds_alternative<int64_t>(value) == true)
			{
				auto val = std::get<int64_t>(value2) + std::get<int64_t>(value);
				value2.emplace<int64_t>(val);
				game.setVariable(key, value2);
			}
			else if (std::holds_alternative<double>(value2) == true &&
				std::holds_alternative<double>(value) == true)
			{
				auto val = std::get<double>(value2) + std::get<double>(value);
				value2.emplace<double>(val);
				game.setVariable(key, value2);
			}
		}
		return true;
	}
};

class ActVariableClear : public Action
{
private:
	std::string key;

public:
	ActVariableClear(const std::string_view key_) : key(key_) {}

	bool execute(Game& game) override
	{
		if (key.empty() == false)
		{
			game.clearVariable(key);
		}
		return true;
	}
};

class ActVariableSave : public Action
{
private:
	std::string filePath;
	std::vector<std::string> vars;

public:
	ActVariableSave(const std::string_view filePath_, std::vector<std::string>&& vars_)
		: filePath(filePath_), vars(std::move(vars_)) {}

	bool execute(Game& game) override
	{
		if (filePath.empty() == false)
		{
			game.saveVariables(filePath, vars);
		}
		return true;
	}
};

class ActVariableSet : public Action
{
private:
	std::string key;
	Variable val;
	bool resolveValue;

public:
	ActVariableSet(const std::string_view key_, const Variable& val_,
		bool resolveValue_) : key(key_), val(val_), resolveValue(resolveValue_) {}

	bool execute(Game& game) override
	{
		if (key.empty() == false)
		{
			if (resolveValue == false)
			{
				game.setVariable(key, val);
			}
			else
			{
				auto var = game.getVarOrProp(val);
				game.setVariable(key, var);
			}
		}
		return true;
	}
};

class ActVariableSetIfNull : public Action
{
private:
	std::string key;
	Variable val;
	bool resolveValue;

public:
	ActVariableSetIfNull(const std::string_view key_, const Variable& val_,
		bool resolveValue_) : key(key_), val(val_), resolveValue(resolveValue_) {}

	bool execute(Game& game) override
	{
		if (key.empty() == false)
		{
			Variable var;
			if (game.getVariableNoToken(key, var) == false)
			{
				if (resolveValue == false)
				{
					game.setVariable(key, val);
				}
				else
				{
					auto var = game.getVarOrProp(val);
					game.setVariable(key, var);
				}
			}
		}
		return true;
	}
};

class ActVariablesSet : public Action
{
private:
	std::vector<std::pair<std::string, Variable>> variables;
	bool resolveValue;

public:
	ActVariablesSet(std::vector<std::pair<std::string, Variable>>&& variables_,
		bool resolveValue_) : variables(std::move(variables_)), resolveValue(resolveValue_) {}

	bool execute(Game& game) override
	{
		for (const auto& keyVar : variables)
		{
			if (keyVar.first.empty() == false)
			{
				if (resolveValue == false)
				{
					game.setVariable(keyVar.first, keyVar.second);
				}
				else
				{
					auto var = game.getVarOrProp(keyVar.second);
					game.setVariable(keyVar.first, var);
				}
			}
		}
		return true;
	}
};
