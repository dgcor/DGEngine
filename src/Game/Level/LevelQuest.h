#pragma once

#include <string_view>

class Level;
class Quest;

class LevelQuest
{
public:
	static void addQuest(Level& level, Quest quest_);

	static void clearQuests(Level& level);

	static void deleteQuest(Level& level, const std::string_view questId);

	static bool hasQuest(const Level& level, const std::string_view questId) noexcept;

	static void setQuestState(Level& level, const std::string_view questId, int state) noexcept;
};
