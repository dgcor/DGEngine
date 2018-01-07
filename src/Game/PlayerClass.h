#pragma once

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
private:
	std::vector<std::shared_ptr<TexturePack>> textures;
	std::array<size_t, (size_t)PlayerAnimation::Size> animationIndexes;

	PlayerAI ai;

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
	PlayerClass() : animationIndexes() {}

	bool hasTextures() const { return textures.empty() == false; }
	void clearTextures() { textures.clear(); }

	TexturePack* getTexturePack(size_t idx) const
	{
		if (idx < textures.size())
		{
			return textures[idx].get();
		}
		else if (textures.empty() == false)
		{
			return textures.front().get();
		}
		return nullptr;
	}

	void addTexturePack(const std::shared_ptr<TexturePack>& texture)
	{
		textures.push_back(texture);
	}

	void clearAnimationIndexes() { animationIndexes.fill(0); }

	size_t getAnimationIndex(PlayerAnimation animation) const
	{
		if (animation < PlayerAnimation::Size)
		{
			return animationIndexes[(size_t)animation];
		}
		return 0;
	}

	void setStatusTexturePackIndex(PlayerAnimation animation, size_t idx)
	{
		if (animation < PlayerAnimation::Size)
		{
			animationIndexes[(size_t)animation] = idx;
		}
	}

	const std::vector<std::pair<uint16_t, Number32>>& Defaults() const { return defaults; }
	void setDefault(const char* prop, const Number32& val);

	AnimationSpeed getSpeed(PlayerAnimation animation) const;
	void setSpeed(PlayerAnimation animation, const AnimationSpeed& speed);

	const PlayerAI& AI() const { return ai; }

	const sf::SoundBuffer* getSound(size_t idx) const;
	const sf::SoundBuffer* getSound(size_t idx, size_t size) const;

	void setSound(size_t idx, const sf::SoundBuffer& snd);

	int16_t getDefaultAttackSound() const { return defaultAttackSound; }
	int16_t getDefaultDefendSound() const { return defaultDefendSound; }
	int16_t getDefaultDieSound() const { return defaultDieSound; }
	int16_t getDefaultHitSound() const { return defaultHitSound; }
	int16_t getDefaultWalkSound() const { return defaultWalkSound; }

	void setDefaultAttackSound(int16_t soundIdx) { defaultAttackSound = soundIdx; }
	void setDefaultDefendSound(int16_t soundIdx) { defaultDefendSound = soundIdx; }
	void setDefaultDieSound(int16_t soundIdx) { defaultDieSound = soundIdx; }
	void setDefaultHitSound(int16_t soundIdx) { defaultHitSound = soundIdx; }
	void setDefaultWalkSound(int16_t soundIdx) { defaultWalkSound = soundIdx; }

	const std::string& Name() const { return name; }
	const std::string& Type() const { return type; }
	const std::string& Description() const { return description; }

	void Name(const std::string& name_) { name = name_; }
	void Type(const std::string& type_) { type = type_; }
	void Description(const std::string& description_) { description = description_; }

	LevelObjValue MaxStrength() const { return maxStrength; }
	LevelObjValue MaxMagic() const { return maxMagic; }
	LevelObjValue MaxDexterity() const { return maxDexterity; }
	LevelObjValue MaxVitality() const { return maxVitality; }

	LevelObjValue MaxResistMagic() const { return maxResistMagic; }
	LevelObjValue MaxResistFire() const { return maxResistFire; }
	LevelObjValue MaxResistLightning() const { return maxResistLightning; }

	uint32_t TotalKills() const { return totalKills; }

	const sf::Color& DefaultOutline() const { return defaultOutline; }
	const sf::Color& DefaultOutlineIgnore() const { return defaultOutlineIgnore; }

	void MaxStrength(LevelObjValue val) { maxStrength = val; }
	void MaxMagic(LevelObjValue val) { maxMagic = val; }
	void MaxDexterity(LevelObjValue val) { maxDexterity = val; }
	void MaxVitality(LevelObjValue val) { maxVitality = val; }

	void MaxResistMagic(LevelObjValue val) { maxResistMagic = val; }
	void MaxResistFire(LevelObjValue val) { maxResistFire = val; }
	void MaxResistLightning(LevelObjValue val) { maxResistLightning = val; }

	void TotalKills(LevelObjValue val) { totalKills = val; }
	void addKill() { totalKills++; }

	void DefaultOutline(const sf::Color& color) { defaultOutline = color; }
	void DefaultOutlineIgnore(const sf::Color& color) { defaultOutlineIgnore = color; }

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
