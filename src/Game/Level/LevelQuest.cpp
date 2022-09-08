#include "LevelQuest.h"
#include "Game/Level/Level.h"

void LevelQuest::addQuest(Level& level, Quest quest_)
{
	for (const auto& quest : level.quests)
	{
		if (quest.Id() == quest_.Id())
		{
			return;
		}
	}
	level.quests.push_back(std::move(quest_));
}

void LevelQuest::deleteQuest(Level& level, const std::string_view questId)
{
	for (auto it = level.quests.begin(); it != level.quests.end(); ++it)
	{
		if (it->Id() == questId)
		{
			level.quests.erase(it);
			return;
		}
	}
}

bool LevelQuest::hasQuest(const Level& level, const std::string_view questId) noexcept
{
	for (auto& quest : level.quests)
	{
		if (quest.Id() == questId)
		{
			return true;
		}
	}
	return false;
}

void LevelQuest::setQuestState(Level& level, const std::string_view questId, int state) noexcept
{
	for (auto& quest : level.quests)
	{
		if (quest.Id() == questId)
		{
			quest.State(state);
			return;
		}
	}
}
