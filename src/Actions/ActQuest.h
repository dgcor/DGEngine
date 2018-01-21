#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/Quest.h"
#include <string>

class ActQuestAdd : public Action
{
private:
	std::string idLevel;
	Quest quest;

public:
	ActQuestAdd(const std::string& idLevel_, Quest quest_)
		: idLevel(idLevel_), quest(std::move(quest_)) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			level->addQuest(quest);
		}
		return true;
	}
};

class ActQuestDelete : public Action
{
private:
	std::string idLevel;
	std::string idQuest;

public:
	ActQuestDelete(const std::string& idLevel_, const std::string& idQuest_)
		: idLevel(idLevel_), idQuest(idQuest_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			level->deleteQuest(idQuest);
		}
		return true;
	}
};

class ActQuestSetState : public Action
{
private:
	std::string idLevel;
	std::string idQuest;
	int state;

public:
	ActQuestSetState(const std::string& idLevel_, const std::string& idQuest_, int state_)
		: idLevel(idLevel_), idQuest(idQuest_), state(state_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(idLevel);
		if (level != nullptr)
		{
			level->setQuestState(idQuest, state);
		}
		return true;
	}
};
