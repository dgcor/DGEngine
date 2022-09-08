#pragma once

#include "Game/AnimationType.h"
#include "Game/BaseAnimation.h"
#include "Game/Classifiers.h"
#include "Game/Formulas.h"
#include "Game/LevelObject/LevelObject.h"
#include "Game/LevelObject/LevelObjectClassDefaults.h"
#include "Game/Properties/AnimationSpeed.h"
#include "Game/Properties/PlayerAnimation.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include "Utils/UnorderedStringMap.h"

class PlayerClass : public LevelObjectClassDefaults<Number32>
{
private:
	std::vector<std::shared_ptr<TexturePack>> textures;
	std::array<uint32_t, (uint32_t)PlayerAnimation::Size> animationIndexes;

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

	UnorderedStringMultiMap<const sf::SoundBuffer*> sounds;

	sf::Color outline{ sf::Color::Transparent };
	sf::Color outlineIgnore{ sf::Color::Transparent };

public:
	PlayerClass() noexcept : animationIndexes() {}

	bool hasTextures() const noexcept { return textures.empty() == false; }
	void clearTextures() noexcept { textures.clear(); }

	void addTexturePack(const std::shared_ptr<TexturePack>& texture)
	{
		textures.push_back(texture);
	}

	std::shared_ptr<TexturePack> getTexturePack(size_t idx) const;

	void setAnimation(size_t textureIdx, PlayerAnimation animation,
		uint32_t direction, BaseAnimation& baseAnim) const;

	void clearAnimationIndexes() { animationIndexes.fill(0); }

	uint32_t getAnimationIndex(PlayerAnimation animation) const noexcept
	{
		if (animation < PlayerAnimation::Size)
		{
			return animationIndexes[(size_t)animation];
		}
		return 0;
	}

	void setStatusTexturePackIndex(PlayerAnimation animation, uint32_t idx) noexcept
	{
		if (animation < PlayerAnimation::Size)
		{
			animationIndexes[(size_t)animation] = idx;
		}
	}

	AnimationSpeed getSpeed(PlayerAnimation animation) const noexcept;
	void setSpeed(PlayerAnimation animation, const AnimationSpeed& speed);

	const sf::SoundBuffer* getSound(const std::string_view key, size_t soundNum = 0) const;

	void addSound(const std::string_view key, const sf::SoundBuffer& snd);

	auto& Name() const noexcept { return name; }
	auto& Type() const noexcept { return type; }

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

	auto MaxStrength() const noexcept { return maxStrength; }
	auto MaxMagic() const noexcept { return maxMagic; }
	auto MaxDexterity() const noexcept { return maxDexterity; }
	auto MaxVitality() const noexcept { return maxVitality; }

	auto MaxResistMagic() const noexcept { return maxResistMagic; }
	auto MaxResistFire() const noexcept { return maxResistFire; }
	auto MaxResistLightning() const noexcept { return maxResistLightning; }

	auto TotalKills() const noexcept { return totalKills; }

	auto& Outline() const noexcept { return outline; }
	auto& OutlineIgnore() const noexcept { return outlineIgnore; }

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
	void deleteFormula(uint16_t nameHash);

	auto getActualLife(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(0, query, default_);
	}
	auto getActualMana(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(1, query, default_);
	}
	auto getActualArmor(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(2, query, default_);
	}
	auto getActualToHit(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(3, query, default_);
	}
	auto getActualDamage(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(4, query, default_);
	}
	auto getActualResistMagic(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(5, query, default_);
	}
	auto getActualResistFire(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(6, query, default_);
	}
	auto getActualResistLightning(const LevelObject& query, LevelObjValue default_) const
	{
		return formulas.eval(7, query, default_);
	}
};
