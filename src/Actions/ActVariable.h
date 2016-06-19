#pragma once

#include "Action.h"
#include "Game.h"
#include "UIObject.h"

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
	ActVariableSet(const std::string& key_, const Variable& val_) : key(key_), val(val_) {}

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
	ActVariableSetId(const std::string& id_, const std::string& key_, const std::string& property_)
		: id(id_), key(key_), property(property_) {}

	virtual bool execute(Game& game)
	{
		if (key.size() > 0)
		{
			auto item = game.Resources().getResource<UIObject>(id);
			if (item != nullptr)
			{
				game.setVariable(key, item->getProperty(property));
			}
		}
		return true;
	}
};
