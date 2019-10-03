#pragma once

#include "Actions/Action.h"
#include "GameHashes.h"
#include "Inventories.h"
#include "LevelObject.h"
#include "MonsterClass.h"
#include "Save/SavePlayer.h"
#include "Spell.h"
#include <SFML/Audio/Sound.hpp>
#include <unordered_map>
#include "Utils/FixedMap.h"
#include "Utils/LRUCache.h"

class Monster : public LevelObject
{
private:
	PairFloat mapPositionMoveTo;
	PairFloat mapPosA;
	PairFloat mapPosB;
	float positionStep = 0.f;
	float currPositionStep = 0.f;
	bool executeActionOnDestination{ false };

	std::vector<PairFloat> walkPath;

	PlayerStatus playerStatus{ PlayerStatus::Stand };

	PlayerDirection playerDirection{ PlayerDirection::All };
	PlayerAnimation playerAnimation{ PlayerAnimation::Size };

	uint16_t restStatus{ 0 };

	size_t textureIdx{ 0 };

	AnimationSpeed speed;
	AnimationSpeed defaultSpeed{ sf::Time::Zero, sf::Time::Zero };

	sf::Time currentWalkTime;

	bool useAI{ false };
	std::shared_ptr<Item> selectedItem;
	const SpellInstance* selectedSpell{ nullptr };

	std::unordered_map<std::string, SpellInstance> spells;

	mutable std::string name;
	mutable std::array<std::string, 2> descriptions;

	mutable bool updateNameAndDescr{ true };

	uint32_t currentLevel{ 0 };

	LevelObjValue life{ 1 };
	LevelObjValue lifeDamage{ 0 };
	LevelObjValue mana{ 0 };
	LevelObjValue manaDamage{ 0 };

	LevelObjValue armor{ 0 };
	LevelObjValue toArmor{ 0 };
	LevelObjValue toHit{ 0 };
	LevelObjValue damageMin{ 0 };
	LevelObjValue damageMax{ 0 };
	LevelObjValue toDamage{ 0 };

	LevelObjValue resistMagic{ 0 };
	LevelObjValue resistMagicItems{ 0 };
	LevelObjValue resistFire{ 0 };
	LevelObjValue resistFireItems{ 0 };
	LevelObjValue resistLightning{ 0 };
	LevelObjValue resistLightningItems{ 0 };

	FixedMap<uint16_t, Number32, 8> customProperties;

	mutable LRUCache<uint16_t, uint32_t, 3> itemQuantityCache;

	sf::Sound currentSound;

	int16_t attackSound{ -1 };
	int16_t defendSound{ -1 };
	int16_t dieSound{ -1 };
	int16_t hitSound{ -1 };
	int16_t walkSound{ -1 };

	void calculateRange();

	void updateAI(Level& level);

	void updateNameAndDescriptions() const;

	void updateSpeed();

	void updateWalkPathStep(PairFloat& newMapPos);
	void updateWalkPath(Game& game, LevelMap& map);

	void updateBodyItemValues();

	void updateAnimation(const Game& game);

	void updateWalk(Game& game, Level& level);
	void updateAttack(Game& game, Level& level);
	void updateDead(Game& game, Level& level);

	bool getCustomIntByHash(uint16_t propHash, Number32& value) const noexcept;
	bool getCustomInt(const std::string_view prop, Number32& value) const noexcept;

	void playSound(int16_t soundIdx);

	friend void Save::serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level, const Player& player);

public:
	Monster(const MonsterClass* class__, const Level& level);

	constexpr const MonsterClass* Class() const noexcept
	{
		return (MonsterClass*)class_;
	}

	using LevelObject::MapPosition;
	bool MapPosition(LevelMap& map, const PairFloat& pos) override;
	using LevelObject::move;
	bool move(LevelMap& map, const PairFloat& pos) override;

	const PairFloat& MapPositionMoveTo() const noexcept { return mapPositionMoveTo; }

	bool getTexture(uint32_t textureNumber, TextureInfo& ti) const override;

	bool getNumberProp(const std::string_view prop, Number32& value) const override;
	bool Passable() const noexcept override { return false; }

	void serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level) const override
	{
		// TODO - If needed.
		//Save::serialize(serializeObj, props, game, level, *this);
	}

	void update(Game& game, Level& level, std::weak_ptr<LevelObject> thisPtr) override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
	void setProperty(const std::string_view prop, const Variable& val) override;
	const Queryable* getQueryable(const std::string_view prop) const override;

	std::vector<std::variant<const Queryable*, Variable>> getQueryableList(
		const std::string_view prop) const;

	const std::string_view getType() const override { return "player"; }

	bool hasIntByHash(uint16_t propHash) const noexcept;
	bool hasInt(const std::string_view prop) const noexcept;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const noexcept;
	bool getInt(const std::string_view prop, LevelObjValue& value) const noexcept;
	bool getUIntByHash(uint16_t propHash, uint32_t& value) const noexcept;
	bool getUInt(const std::string_view prop, uint32_t& value) const noexcept;
	bool setIntByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept;
	bool setInt(const std::string_view prop, LevelObjValue value, const Level* level) noexcept;
	bool setUIntByHash(uint16_t propHash, uint32_t value, const Level* level) noexcept;
	bool setUInt(const std::string_view prop, uint32_t value, const Level* level) noexcept;

	bool getNumberByHash(uint16_t propHash, const std::string_view props, Number32& value) const noexcept;
	bool setNumberByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept;
	bool setNumberByHash(uint16_t propHash, const Number32& value, const Level* level) noexcept;
	bool setNumber(const std::string_view prop, LevelObjValue value, const Level* level) noexcept;
	bool setNumber(const std::string_view prop, const Number32& value, const Level* level) noexcept;

	void clearWalkPath() noexcept { walkPath.clear(); }
	void setWalkPath(const std::vector<PairFloat>& walkPath_, bool doAction);
	void Walk(const LevelMap& map, const PairFloat& walkToMapPos, bool doAction);
	void Walk(const LevelMap& map, const PlayerDirection direction, bool doAction);

	void setDefaultSpeed(const AnimationSpeed& speed_);

	PlayerDirection getDirection() const noexcept { return playerDirection; }

	void setDirection(PlayerDirection direction_);

	void setAnimation(PlayerAnimation animation_);

	void setTextureIdx(size_t idx_);

	void setRestStatus(uint16_t restStatus_) noexcept;

	void setStatus(PlayerStatus status_) noexcept;

	void setStandAnimation()
	{
		setAnimation((PlayerAnimation)((size_t)PlayerAnimation::Stand1 + restStatus));
	}
	void setWalkAnimation()
	{
		setAnimation((PlayerAnimation)((size_t)PlayerAnimation::Walk1 + restStatus));
	}
	bool hasWalkingAnimation() noexcept
	{
		return playerAnimation >= PlayerAnimation::Walk1 &&
			playerAnimation <= PlayerAnimation::Walk2;
	}

	void resetAnimationTime() noexcept { animation.elapsedTime.currentTime = speed.animation; }

	Item* SelectedItem() const noexcept { return selectedItem.get(); }
	std::shared_ptr<Item> SelectedItem(std::shared_ptr<Item> item) noexcept;

	void SelectedSpell(const std::string& id) noexcept;

	bool hasSpell(const std::string& key) const;
	void addSpell(const std::string key, Spell* obj, LevelObjValue spellLevel = 1);
	Spell* getSpell(const std::string& key) const;
	const SpellInstance* getSpellInstance(const std::string& key) const;

	void updateProperties();

	void applyDefaults(const Level& level) noexcept;

	template <class T>
	bool canUseObject(const T& obj) const
	{
		LevelObjValue value;
		if (obj.getNumberPropByHash(*this, ItemProp::RequiredLife, value) == true &&
			value > LifeNow())
		{
			return false;
		}
		if (obj.getNumberPropByHash(*this, ItemProp::RequiredMana, value) == true &&
			value > ManaNow())
		{
			return false;
		}
		return true;
	}

	bool isAI() const noexcept { return useAI; }
	void setAI(bool ai_) noexcept { useAI = ai_; }

	void setAttackSounds(int16_t soundIdx) noexcept { attackSound = soundIdx; }
	void setDefendSounds(int16_t soundIdx) noexcept { defendSound = soundIdx; }
	void setDieSounds(int16_t soundIdx) noexcept { dieSound = soundIdx; }
	void setHitSounds(int16_t soundIdx) noexcept { hitSound = soundIdx; }
	void setWalkSounds(int16_t soundIdx) noexcept { walkSound = soundIdx; }

	const std::string& Id() const noexcept { return id; }
	const std::string& Name() const;
	const std::string& SimpleName() const noexcept { return Class()->Name(); }

	LevelObjValue LifeNow() const noexcept { return life - lifeDamage; }
	LevelObjValue ManaNow() const noexcept { return mana - manaDamage; }

	void Id(const std::string_view id_) { id = id_; }
	void Name(const std::string_view name_) { name = name_; }

	void CurrentLevel(uint32_t level_) noexcept { currentLevel = level_; }

	void Life(LevelObjValue life_) noexcept { life = life_; }
	void LifeDamage(LevelObjValue lifeDamage_) noexcept { lifeDamage = lifeDamage_; }
	void Mana(LevelObjValue manaBase_) noexcept { mana = manaBase_; }
	void ManaDamage(LevelObjValue manaDamage_) noexcept { manaDamage = manaDamage_; }

	void Armor(LevelObjValue armor_) noexcept { armor = armor_; }
	void ToHit(LevelObjValue toHit_) noexcept { toHit = toHit_; }

	void ResistMagic(LevelObjValue resistMagic_) noexcept { resistMagic = resistMagic_; }
	void ResistFire(LevelObjValue resistFire_) noexcept { resistFire = resistFire_; }
	void ResistLightning(LevelObjValue resistLightning_) noexcept { resistLightning = resistLightning_; }
};
