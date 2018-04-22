#pragma once

#include "AnimationType.h"
#include "BaseAnimation.h"
#include "BaseClass.h"
#include "Formula.h"
#include "GameProperties.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include "TexturePacks/TexturePack.h"

struct PlayerAI
{
	bool walksSlowly{ false };
	int16_t sightRadius{ 0 };
};

class PlayerClass : public BaseClass
{
public:
	struct Range
	{
		std::pair<size_t, size_t> range;
		AnimationType animType{ AnimationType::PlayOnce };
	};
	typedef std::vector<Range> Ranges;

private:
	std::vector<std::pair<std::shared_ptr<TexturePack>, Ranges>> textures;
	std::array<size_t, (size_t)PlayerAnimation::Size> animationIndexes;

	PlayerAI ai;

	std::string id;
	std::string name;
	std::string type;
	std::string description;

	std::vector<std::pair<uint16_t, Number32>> defaults;

	LevelObjValue maxStrength{ 0 };
	LevelObjValue maxMagic{ 0 };
	LevelObjValue maxDexterity{ 0 };
	LevelObjValue maxVitality{ 0 };

	LevelObjValue maxResistMagic{ 0 };
	LevelObjValue maxResistFire{ 0 };
	LevelObjValue maxResistLightning{ 0 };

	uint32_t totalKills{ 0 };

	std::vector<std::pair<PlayerAnimation, AnimationSpeed>> animationSpeeds;

	std::array<Formula, 8> formulas;

	std::array<const sf::SoundBuffer*, 16> sounds{ {nullptr} };

	int16_t defaultAttackSound{ -1 };
	int16_t defaultDefendSound{ -1 };
	int16_t defaultDieSound{ -1 };
	int16_t defaultHitSound{ -1 };
	int16_t defaultWalkSound{ -1 };

	sf::Color defaultOutline{ sf::Color::Transparent };
	sf::Color defaultOutlineIgnore{ sf::Color::Transparent };

	LevelObjValue evalFormula(size_t idx,
		const LevelObject& query, LevelObjValue default_) const;

public:
	PlayerClass() noexcept : animationIndexes() {}

	bool hasTextures() const noexcept { return textures.empty() == false; }
	void clearTextures() noexcept { textures.clear(); }

	TexturePack* getTexturePack(size_t idx) const;

	void getTextureAnimationRange(size_t idx,
		PlayerAnimation animation, BaseAnimation& baseAnim) const;

	void addTexturePack(const std::shared_ptr<TexturePack>& texture, Ranges ranges = {})
	{
		textures.push_back(std::make_pair(std::move(texture), std::move(ranges)));
	}

	void clearAnimationIndexes() { animationIndexes.fill(0); }

	size_t getAnimationIndex(PlayerAnimation animation) const noexcept
	{
		if (animation < PlayerAnimation::Size)
		{
			return animationIndexes[(size_t)animation];
		}
		return 0;
	}

	void setStatusTexturePackIndex(PlayerAnimation animation, size_t idx) noexcept
	{
		if (animation < PlayerAnimation::Size)
		{
			animationIndexes[(size_t)animation] = idx;
		}
	}

	const std::vector<std::pair<uint16_t, Number32>>& Defaults() const noexcept { return defaults; }
	void setDefault(const char* prop, const Number32& val);
	void setDefaultByHash(uint16_t propHash, const Number32& val);

	AnimationSpeed getSpeed(PlayerAnimation animation) const noexcept;
	void setSpeed(PlayerAnimation animation, const AnimationSpeed& speed);

	const PlayerAI& AI() const noexcept { return ai; }

	const sf::SoundBuffer* getSound(size_t idx) const noexcept;
	const sf::SoundBuffer* getSound(size_t idx, size_t size) const;

	void setSound(size_t idx, const sf::SoundBuffer& snd) noexcept;

	int16_t getDefaultAttackSound() const noexcept { return defaultAttackSound; }
	int16_t getDefaultDefendSound() const noexcept { return defaultDefendSound; }
	int16_t getDefaultDieSound() const noexcept { return defaultDieSound; }
	int16_t getDefaultHitSound() const noexcept { return defaultHitSound; }
	int16_t getDefaultWalkSound() const noexcept { return defaultWalkSound; }

	void setDefaultAttackSound(int16_t soundIdx) noexcept { defaultAttackSound = soundIdx; }
	void setDefaultDefendSound(int16_t soundIdx) noexcept { defaultDefendSound = soundIdx; }
	void setDefaultDieSound(int16_t soundIdx) noexcept { defaultDieSound = soundIdx; }
	void setDefaultHitSound(int16_t soundIdx) noexcept { defaultHitSound = soundIdx; }
	void setDefaultWalkSound(int16_t soundIdx) noexcept { defaultWalkSound = soundIdx; }

	const std::string& Id() const noexcept { return id; }
	const std::string& Name() const noexcept { return name; }
	const std::string& Type() const noexcept { return type; }
	const std::string& Description() const noexcept { return description; }

	void Id(const std::string& id_) { id = id_; }
	void Name(const std::string& name_) { name = name_; }
	void Type(const std::string& type_) { type = type_; }
	void Description(const std::string& description_) { description = description_; }

	LevelObjValue MaxStrength() const noexcept { return maxStrength; }
	LevelObjValue MaxMagic() const noexcept { return maxMagic; }
	LevelObjValue MaxDexterity() const noexcept { return maxDexterity; }
	LevelObjValue MaxVitality() const noexcept { return maxVitality; }

	LevelObjValue MaxResistMagic() const noexcept { return maxResistMagic; }
	LevelObjValue MaxResistFire() const noexcept { return maxResistFire; }
	LevelObjValue MaxResistLightning() const noexcept { return maxResistLightning; }

	uint32_t TotalKills() const noexcept { return totalKills; }

	const sf::Color& DefaultOutline() const noexcept { return defaultOutline; }
	const sf::Color& DefaultOutlineIgnore() const noexcept { return defaultOutlineIgnore; }

	void MaxStrength(LevelObjValue val) noexcept { maxStrength = val; }
	void MaxMagic(LevelObjValue val) noexcept { maxMagic = val; }
	void MaxDexterity(LevelObjValue val) noexcept { maxDexterity = val; }
	void MaxVitality(LevelObjValue val) noexcept { maxVitality = val; }

	void MaxResistMagic(LevelObjValue val) noexcept { maxResistMagic = val; }
	void MaxResistFire(LevelObjValue val) noexcept { maxResistFire = val; }
	void MaxResistLightning(LevelObjValue val) noexcept { maxResistLightning = val; }

	void TotalKills(LevelObjValue val) noexcept { totalKills = val; }
	void addKill() noexcept { totalKills++; }

	void DefaultOutline(const sf::Color& color) noexcept { defaultOutline = color; }
	void DefaultOutlineIgnore(const sf::Color& color) noexcept { defaultOutlineIgnore = color; }

	void setLifeFormula(const Formula& formula) { formulas[0] = formula; }
	void setManaFormula(const Formula& formula) { formulas[1] = formula; }
	void setArmorFormula(const Formula& formula) { formulas[2] = formula; }
	void setToHitFormula(const Formula& formula) { formulas[3] = formula; }
	void setDamageFormula(const Formula& formula) { formulas[4] = formula; }
	void setResistMagicFormula(const Formula& formula) { formulas[5] = formula; }
	void setResistFireFormula(const Formula& formula) { formulas[6] = formula; }
	void setResistLightningFormula(const Formula& formula) { formulas[7] = formula; }

	LevelObjValue getActualLife(const LevelObject& query, LevelObjValue default_) const
	{
		return evalFormula(0, query, default_);
	}
	LevelObjValue getActualMana(const LevelObject& query, LevelObjValue default_) const
	{
		return evalFormula(1, query, default_);
	}
	LevelObjValue getActualArmor(const LevelObject& query, LevelObjValue default_) const
	{
		return evalFormula(2, query, default_);
	}
	LevelObjValue getActualToHit(const LevelObject& query, LevelObjValue default_) const
	{
		return evalFormula(3, query, default_);
	}
	LevelObjValue getActualDamage(const LevelObject& query, LevelObjValue default_) const
	{
		return evalFormula(4, query, default_);
	}
	LevelObjValue getActualResistMagic(const LevelObject& query, LevelObjValue default_) const
	{
		return evalFormula(5, query, default_);
	}
	LevelObjValue getActualResistFire(const LevelObject& query, LevelObjValue default_) const
	{
		return evalFormula(6, query, default_);
	}
	LevelObjValue getActualResistLightning(const LevelObject& query, LevelObjValue default_) const
	{
		return evalFormula(7, query, default_);
	}
};
