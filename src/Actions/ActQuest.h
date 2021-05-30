#pragma once

#include "Actions/Action.h"
#include "Game.h"
#include "Game/Quest.h"

class ActQuestAdd : public Action
{
private:
	std::string idLevel;
	Quest quest;

public:
	ActQuestAdd(const std::string_view idLevel_, Quest quest_)
		: idLevel(idLevel_), quest(std::move(quest_)) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
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
	ActQuestDelete(const std::string_view idLevel_, const std::string_view idQuest_)
		: idLevel(idLevel_), idQuest(idQuest_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
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
	ActQuestSetState(const std::string_view idLevel_, const std::string_view idQuest_, int state_)
		: idLevel(idLevel_), idQuest(idQuest_), state(state_) {}

	bool execute(Game& game) noexcept override
	{
		auto level = game.Resources().getLevel<Level>(idLevel);
		if (level != nullptr)
		{
			level->setQuestState(idQuest, state);
		}
		return true;
	}
};
