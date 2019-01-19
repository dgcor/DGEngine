#pragma once

#include "AnimationType.h"
#include "BaseAnimation.h"
#include "Classifiers.h"
#include "Formulas.h"
#include "GameProperties.h"
#include "LevelObjectClassDefaults.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include "TexturePacks/TexturePack.h"

struct PlayerAI
{
	bool walksSlowly{ false };
	int16_t sightRadius{ 0 };
};

class PlayerClass : public LevelObjectClassDefaults<Number32>
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

	std::string name;
	std::string type;

	Classifiers<3> classifiers;

	LevelObjValue maxStrength{ 0 };
	LevelObjValue maxMagic{ 0 };
	LevelObjValue maxDexterity{ 0 };
	LevelObjValue maxVitality{ 0 };

	LevelObjValue maxResistMagic{ 0 };
	LevelObjValue maxResistFire{ 0 };
	LevelObjValue maxResistLightning{ 0 };

	uint32_t totalKills{ 0 };

	std::vector<std::pair<PlayerAnimation, AnimationSpeed>> animationSpeeds;

	Formulas<std::array<Formula, 8>> formulas;

	std::array<const sf::SoundBuffer*, 16> sounds{ {nullptr} };

	int16_t attackSound{ -1 };
	int16_t defendSound{ -1 };
	int16_t dieSound{ -1 };
	int16_t hitSound{ -1 };
	int16_t walkSound{ -1 };

	sf::Color outline{ sf::Color::Transparent };
	sf::Color outlineIgnore{ sf::Color::Transparent };

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

	AnimationSpeed getSpeed(PlayerAnimation animation) const noexcept;
	void setSpeed(PlayerAnimation animation, const AnimationSpeed& speed);

	const PlayerAI& AI() const noexcept { return ai; }

	const sf::SoundBuffer* getSound(size_t idx) const noexcept;
	const sf::SoundBuffer* getSound(size_t idx, size_t size) const;

	void setSound(size_t idx, const sf::SoundBuffer& snd) noexcept;

	int16_t getAttackSound() const noexcept { return attackSound; }
	int16_t getDefendSound() const noexcept { return defendSound; }
	int16_t getDieSound() const noexcept { return dieSound; }
	int16_t getHitSound() const noexcept { return hitSound; }
	int16_t getWalkSound() const noexcept { return walkSound; }

	void setAttackSound(int16_t soundIdx) noexcept { attackSound = soundIdx; }
	void setDefendSound(int16_t soundIdx) noexcept { defendSound = soundIdx; }
	void setDieSound(int16_t soundIdx) noexcept { dieSound = soundIdx; }
	void setHitSound(int16_t soundIdx) noexcept { hitSound = soundIdx; }
	void setWalkSound(int16_t soundIdx) noexcept { walkSound = soundIdx; }

	const std::string& Name() const noexcept { return name; }
	const std::string& Type() const noexcept { return type; }

	void Name(const std::string_view name_) { name = name_; }
	void Type(const std::string_view type_) { type = type_; }

	bool getFullName(const Queryable& obj, std::string& fullName) const
	{
		return classifiers.getText(0, obj, fullName);
	}
	void setNameClassifier(Classifier* classifier) { classifiers.set(0, classifier, 0); }

	bool getDescription(size_t idx, const Queryable& obj, std::string& description) const
	{
		return classifiers.getText(idx + 1, obj, description);
	}
	void setDescription(size_t idx, Classifier* classifier, uint16_t skipFirst)
	{
		classifiers.set(idx + 1, classifier, skipFirst);
	}

	LevelObjValue MaxStrength() const noexcept { return maxStrength; }
	LevelObjValue MaxMagic() const noexcept { return maxMagic; }
	LevelObjValue MaxDexterity() const noexcept { return maxDexterity; }
	LevelObjValue MaxVitality() const noexcept { return maxVitality; }

	LevelObjValue MaxResistMagic() const noexcept { return maxResistMagic; }
	LevelObjValue MaxResistFire() const noexcept { return maxResistFire; }
	LevelObjValue MaxResistLightning() const noexcept { return maxResistLightning; }

	uint32_t TotalKills() const noexcept { return totalKills; }

	const sf::Color& Outline() const noexcept { return outline; }
	const sf::Color& OutlineIgnore() const noexcept { return outlineIgnore; }

	void MaxStrength(LevelObjValue val) noexcept { maxStrength = val; }
	void MaxMagic(LevelObjValue val) noexcept { maxMagic = val; }
	void MaxDexterity(LevelObjValue val) noexcept { maxDexterity = val; }
	void MaxVitality(LevelObjValue val) noexcept { maxVitality = val; }

	void MaxResistMagic(LevelObjValue val) noexcept { maxResistMagic = val; }
	void MaxResistFire(LevelObjValue val) noexcept { maxResistFire = val; }
	void MaxResistLightning(LevelObjValue val) noexcept { maxResistLightning = val; }

	void TotalKills(LevelObjValue val) noexcept { totalKills = val; }
	void addKill() noexcept { totalKills++; }

	void Outline(const sf::Color& color) noexcept { outline = color; }
	void OutlineIgnore(const sf::Color& color) noexcept { outlineIgnore = color; }

	// if formula is empty, it clears the current formula.
	void setFormula(uint16_t nameHash, const std::string_view formula);

	LevelObjValue getActualLife(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(0, query, default_);
	}
	LevelObjValue getActualMana(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(1, query, default_);
	}
	LevelObjValue getActualArmor(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(2, query, default_);
	}
	LevelObjValue getActualToHit(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(3, query, default_);
	}
	LevelObjValue getActualDamage(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(4, query, default_);
	}
	LevelObjValue getActualResistMagic(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(5, query, default_);
	}
	LevelObjValue getActualResistFire(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(6, query, default_);
	}
	LevelObjValue getActualResistLightning(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(7, query, default_);
	}
};
