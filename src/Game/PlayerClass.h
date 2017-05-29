#pragma once

#include "CelCache.h"
#include "Formula.h"
#include "GameProperties.h"
#include <memory>
#include <string>

class PlayerClass
{
private:
	std::array<std::shared_ptr<CelTextureCacheVector>, PlayerPaletteSize> celTextures;
	std::array<size_t, (size_t)PlayerStatus::Size> statusCelIndexes;

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

	std::vector<std::pair<PlayerStatus, AnimationSpeed>> animationSpeeds;

	std::array<Formula, 5> formulas;

	LevelObjValue evalFormula(size_t idx,
		const LevelObject& query, LevelObjValue default_) const;

public:
	PlayerClass() : statusCelIndexes() {}

	CelTextureCacheVector* getCelTexture(size_t paletteIdx) const
	{
		if (paletteIdx < PlayerPaletteSize)
		{
			return celTextures[paletteIdx].get();
		}
		return nullptr;
	}

	void setCelTexture(size_t paletteIdx,
		const std::shared_ptr<CelTextureCacheVector>& celTexture)
	{
		if (paletteIdx < PlayerPaletteSize)
		{
			celTextures[paletteIdx] = celTexture;
		}
	}

	void clearStatusCelIndexes() { statusCelIndexes.fill(0); }

	size_t getStatusCelIndex(PlayerStatus status) const
	{
		if (status < PlayerStatus::Size)
		{
			return statusCelIndexes[(size_t)status];
		}
		return 0;
	}

	void setStatusCelIndex(PlayerStatus status, size_t idx)
	{
		if (status < PlayerStatus::Size)
		{
			statusCelIndexes[(size_t)status] = idx;
		}
	}

	const std::array<std::shared_ptr<CelTextureCacheVector>, PlayerPaletteSize>& CelTextures() const
	{
		return celTextures;
	}

	const std::vector<std::pair<uint16_t, Number32>>& Defaults() const { return defaults; }
	void setDefault(const char* prop, const Number32& val);

	AnimationSpeed getSpeed(PlayerStatus status) const;
	void setSpeed(PlayerStatus status, const AnimationSpeed& speed);

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

	void MaxStrength(LevelObjValue val) { maxStrength = val; }
	void MaxMagic(LevelObjValue val) { maxMagic = val; }
	void MaxDexterity(LevelObjValue val) { maxDexterity = val; }
	void MaxVitality(LevelObjValue val) { maxVitality = val; }

	void MaxResistMagic(LevelObjValue val) { maxResistMagic = val; }
	void MaxResistFire(LevelObjValue val) { maxResistFire = val; }
	void MaxResistLightning(LevelObjValue val) { maxResistLightning = val; }

	void setLifeFormula(const Formula& formula) { formulas[0] = formula; }
	void setManaFormula(const Formula& formula) { formulas[1] = formula; }
	void setArmorFormula(const Formula& formula) { formulas[2] = formula; }
	void setToHitFormula(const Formula& formula) { formulas[3] = formula; }
	void setDamageFormula(const Formula& formula) { formulas[4] = formula; }

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
};
