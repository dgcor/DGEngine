#pragma once

#include "Game/ActionQueryable.h"
#include "Game/Item/ItemLocation.h"
#include "Game/VarOrQueryObject.h"
#include "LevelBase.h"

class Level : public ActionQueryable, public LevelBase
{
private:
	friend class LevelDraw;
	friend class LevelDrawableManager;
	friend class LevelInputManager;
	friend class LevelItem;
	friend class LevelQuest;
	friend class LevelSave;
	friend class LevelUIObject;

public:
	void save(const std::string_view filePath, const UnorderedStringMap<Variable>& props) const;

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
	QueryObject getQueryable(const std::string_view prop) const override;

	std::vector<VarOrQueryObject> getQueryableList(const std::string_view prop) const;

	void draw(const Game& game, sf::RenderTarget& target) const override;
	void update(Game& game) override;

	// doesn't clear currently used player classes
	void clearPlayerClasses();
	// doesn't clear currently used player classes
	void clearPlayerTextures() noexcept;

	std::shared_ptr<Player> getPlayerOrCurrent(const std::string_view id) const noexcept;

	void setCurrentPlayer(std::weak_ptr<Player> player_) noexcept;

	bool FollowCurrentPlayer() const noexcept { return followCurrentPlayer; }
	void FollowCurrentPlayer(bool follow) noexcept { followCurrentPlayer = follow; }

	// only updates if level view is following current player
	bool updateCurrentMapViewCenter(bool smooth) noexcept;

	Item* getItem(const PairFloat& mapCoord) const noexcept;
	Item* getItem(const ItemCoordInventory& itemCoord) const;
	Item* getItem(const ItemCoordInventory& itemCoord, std::shared_ptr<Player>& player) const;
	Item* getItem(const ItemLocation& location) const;
	Item* getItem(const ItemLocation& location, std::shared_ptr<Player>& player) const;

	std::shared_ptr<Item> removeItem(const PairFloat& mapCoord);
	std::shared_ptr<Item> removeItem(const ItemCoordInventory& itemCoord);
	std::shared_ptr<Item> removeItem(const ItemCoordInventory& itemCoord, std::shared_ptr<Player>& player);
	std::shared_ptr<Item> removeItem(const ItemLocation& location);
	std::shared_ptr<Item> removeItem(const ItemLocation& location, std::shared_ptr<Player>& player);

	bool setItem(const PairFloat& mapCoord, std::shared_ptr<Item>& item);
	bool setItem(const ItemCoordInventory& itemCoord, std::shared_ptr<Item>& item);
	bool setItem(const ItemLocation& location, std::shared_ptr<Item>& item);

	bool deleteItem(const PairFloat& mapCoord);
	bool deleteItem(const ItemCoordInventory& itemCoord);
	bool deleteItem(const ItemLocation& location);

	// returns the remaining quantity to add/remove. 0 if all quantity was added.
	LevelObjValue addItemQuantity(const ItemLocation& location, LevelObjValue amount);

	auto& Quests() const noexcept { return quests; };
	void addQuest(Quest quest_);
	void clearQuests();
	void deleteQuest(const std::string_view questId);
	bool hasQuest(const std::string_view questId) const noexcept;
	void setQuestState(const std::string_view questId, int state) noexcept;

	bool hasSpell(const std::string_view id) const;
	Spell* getSpell(const std::string_view id) const;

	void setExperiencePoints(const std::vector<uint32_t>& experiencePoints_) { experiencePoints = experiencePoints_; }

	uint32_t getExperienceFromLevel(uint32_t level) const;
	uint32_t getLevelFromExperience(uint32_t experience) const;
};
