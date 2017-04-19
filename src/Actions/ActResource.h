#pragma once

#include "Action.h"
#include "Game.h"
#include "Parser/Parser.h"
#include <string>

class ActResourceAdd : public Action
{
private:
	std::string id;
	IgnoreResource ignorePrevious;
	bool hasIgnore{ false };

public:
	ActResourceAdd(const std::string& id_) : id(id_) {}

	void setIgnorePrevious(IgnoreResource ignore)
	{
		ignorePrevious = ignore;
		hasIgnore = true;
	}

	virtual bool execute(Game& game)
	{
		if (hasIgnore == true)
		{
			game.Resources().ignoreTopResource(ignorePrevious);
		}
		game.Resources().addResource(id);
		return true;
	}
};

class ActResourceIgnore : public Action
{
private:
	std::string id;
	IgnoreResource ignore;

public:
	ActResourceIgnore(const std::string& id_, IgnoreResource ignorePrevious_)
		: id(id_), ignore(ignorePrevious_) {}

	virtual bool execute(Game& game)
	{
		game.Resources().ignoreResources(id, ignore);
		return true;
	}
};

class ActResourcePop : public Action
{
private:
	std::string id;
	IgnoreResource ignorePrevious;

public:
	ActResourcePop(const std::string& id_, IgnoreResource ignorePrevious_)
		: id(id_), ignorePrevious(ignorePrevious_) {}

	virtual bool execute(Game& game)
	{
		if (id.size() > 0)
		{
			game.Resources().popResource(id);
		}
		else
		{
			game.Resources().popResource();
		}
		game.Resources().ignoreTopResource(ignorePrevious);
		return true;
	}
};

class ActResourcePopAll : public Action
{
private:
	std::string id;
	bool popBaseResources;
	IgnoreResource ignorePrevious;

public:
	ActResourcePopAll(const std::string& id_, bool popBaseResources_, IgnoreResource ignorePrevious_)
		: id(id_), popBaseResources(popBaseResources_), ignorePrevious(ignorePrevious_) {}

	virtual bool execute(Game& game)
	{
		if (id.size() > 0)
		{
			game.Resources().popAllResources(id);
		}
		else
		{
			game.Resources().popAllResources(popBaseResources);
		}
		game.Resources().ignoreTopResource(ignorePrevious);
		return true;
	}
};
