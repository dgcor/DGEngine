#pragma once

#include "LevelBase.h"
#include "ItemLocation.h"
#include "Save/SaveLevel.h"

class Level : public LevelBase
{
private:
	friend void Save::save(const std::string_view filePath,
		const Save::Properties& props, const Game& game, const Level& level);
	friend void Save::serialize(void* serializeObj,
		const Save::Properties& props, const Game& game, const Level& level);

	void processInputEvents(Game& game);

public:
	void save(const std::string_view filePath,
		const Save::Properties& props, const Game& game) const;
	void serialize(void* serializeObj, const Save::Properties& props,
		const Game& game, const Level& level);

	bool getProperty(const std::string_view prop, Variable& var) const override;
	const Queryable* getQueryable(const std::string_view prop) const override;

	std::vector<std::variant<const Queryable*, Variable>> getQueryableList(
		const std::string_view prop) const;

	void draw(const Game& game, sf::RenderTarget& target) const override;
	void update(Game& game) override;

	// doesn't clear currently used player classes
	void clearPlayerClasses();
	// doesn't clear currently used player classes
	void clearPlayerTextures() noexcept;

	Player* getPlayerOrCurrent(const std::string_view id) const noexcept;

	Player* getCurrentPlayer() const noexcept { return currentPlayer.lock().get(); }
	void setCurrentPlayer(std::weak_ptr<Player> player_) noexcept;

	bool FollowCurrentPlayer() const noexcept { return followCurrentPlayer; }
	void FollowCurrentPlayer(bool follow) noexcept { followCurrentPlayer = follow; }

	// only updates if level view is following current player
	bool updateCurrentMapViewCenter(bool smooth) noexcept;

	Item* getItem(const PairFloat& mapCoord) const noexcept;
	Item* getItem(const ItemCoordInventory& itemCoord) const;
	Item* getItem(const ItemCoordInventory& itemCoord, Player*& player) const;
	Item* getItem(const ItemLocation& location) const;
	Item* getItem(const ItemLocation& location, Player*& player) const;

	std::shared_ptr<Item> removeItem(const PairFloat& mapCoord);
	std::shared_ptr<Item> removeItem(const ItemCoordInventory& itemCoord);
	std::shared_ptr<Item> removeItem(const ItemCoordInventory& itemCoord, Player*& player);
	std::shared_ptr<Item> removeItem(const ItemLocation& location);
	std::shared_ptr<Item> removeItem(const ItemLocation& location, Player*& player);

	bool setItem(const PairFloat& mapCoord, std::shared_ptr<Item>& item);
	bool setItem(const ItemCoordInventory& itemCoord, std::shared_ptr<Item>& item);
	bool setItem(const ItemLocation& location, std::shared_ptr<Item>& item);

	bool deleteItem(const PairFloat& mapCoord);
	bool deleteItem(const ItemCoordInventory& itemCoord);
	bool deleteItem(const ItemLocation& location);

	// returns the remaining quantity to add/remove. 0 if all quantity was added.
	LevelObjValue addItemQuantity(const ItemLocation& location, LevelObjValue amount);

	const std::vector<Quest>& Quests() const noexcept { return quests; };
	void addQuest(Quest quest_);
	void clearQuests();
	void deleteQuest(const std::string_view questId);
	bool hasQuest(const std::string_view questId) const noexcept;
	void setQuestState(const std::string_view questId, int state) noexcept;

	bool hasSpell(const std::string_view id) const;
	Spell* getSpell(const std::string_view id) const;

	void setExperiencePoints(const std::vector<uint32_t>& experiencePoints_)
	{
		experiencePoints = experiencePoints_;
	}

	uint32_t getExperienceFromLevel(uint32_t level) const;
	uint32_t getLevelFromExperience(uint32_t experience) const;
};
