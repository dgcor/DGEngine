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
	ActVariableAdd(const std::string& key_, const Variable& value_)
		: key(key_), value(value_) {}

	virtual bool execute(Game& game)
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
	ActVariableClear(const std::string& key_) : key(key_) {}

	virtual bool execute(Game& game)
	{
		if (key.size() > 0)
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
	ActVariableSave(const std::string& filePath_, const std::vector<std::string>& vars_)
		: filePath(filePath_), vars(vars_) {}

	virtual bool execute(Game& game)
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

public:
	ActVariableSet(const std::string& key_, const Variable& val_)
		: key(key_), val(val_) {}

	virtual bool execute(Game& game)
	{
		if (key.size() > 0)
		{
			game.setVariable(key, val);
		}
		return true;
	}
};

class ActVariableSetId : public Action
{
private:
	std::string id;
	std::string key;
	std::string property;

public:
	ActVariableSetId(const std::string& id_, const std::string& key_,
		const std::string& property_) : id(id_), key(key_), property(property_) {}

	virtual bool execute(Game& game)
	{
		if (key.size() > 0)
		{
			auto item = game.Resources().getResource<UIObject>(id);
			if (item != nullptr)
			{
				Variable var;
				if (item->getProperty(property, var) == true)
				{
					game.setVariable(key, var);
				}
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

public:
	ActVariableSetIfNull(const std::string& key_, const Variable& val_)
		: key(key_), val(val_) {}

	virtual bool execute(Game& game)
	{
		if (key.size() > 0)
		{
			Variable var;
			if (game.getVariableNoToken(key, var) == false)
			{
				game.setVariable(key, val);
			}
		}
		return true;
	}
};
