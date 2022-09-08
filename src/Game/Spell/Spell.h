#pragma once

#include "Game/Classifiers.h"
#include "Game/Formulas.h"
#include "Game/LevelObject/LevelObjectClass.h"
#include "Game/LevelObject/LevelObjectQueryable.h"
#include "Game/Queryable.h"
#include "Game/TextureQueryable.h"
#include "Resources/TexturePack.h"
#include <string>
#include "Utils/FixedMap.h"

class Spell : public LevelObjectClass, public LevelObjectQueryable
{
private:
	std::string name;
	std::string spellType;

	FixedMap<uint16_t, LevelObjValue, 6> properties;

	std::shared_ptr<TexturePack> texturePack1;
	std::shared_ptr<TexturePack> texturePack2;
	uint32_t textureIdx1;
	uint32_t textureIdx2;

	Formulas<std::array<Formula, 6>> formulas;
	FixedMap<uint16_t, Formula, 4> customFormulas;

	static constexpr size_t LifeFormula = 0;
	static constexpr size_t ManaFormula = 1;
	static constexpr size_t DamageFormula = 2;
	static constexpr size_t DurationFormula = 3;
	static constexpr size_t SpeedFormula = 4;

	mutable std::array<std::string, 5> descriptions;
	mutable bool updateDescr{ true };

	Classifiers<5> descriptionClassifiers;

	bool getDescription(size_t idx, const Queryable& obj, std::string& description) const;
	void updateDescriptions(const Queryable& spellObj) const;

	bool evalFormula(uint16_t nameHash, const Queryable& objA,
		const Queryable& objB, LevelObjValue& val,
		const std::string_view minMaxNumber = {}) const;

public:
	auto begin() noexcept { return properties.begin(); }
	auto end() noexcept { return properties.end(); }
	auto begin() const noexcept { return properties.begin(); }
	auto end() const noexcept { return properties.end(); }
	auto cbegin() const noexcept { return properties.cbegin(); }
	auto cend() const noexcept { return properties.cend(); }
	auto rbegin() noexcept { return properties.rbegin(); }
	auto rend() noexcept { return properties.rend(); }
	auto rbegin() const noexcept { return properties.rbegin(); }
	auto rend() const noexcept { return properties.rend(); }
	auto crbegin() const noexcept { return properties.crbegin(); }
	auto crend() const noexcept { return properties.crend(); }

	Spell(const std::shared_ptr<TexturePack>& texturePack1_,
		const std::shared_ptr<TexturePack>& texturePack2_,
		uint32_t textureIdx1_, uint32_t textureIdx2_) : texturePack1(texturePack1_),
		texturePack2(texturePack2_), textureIdx1(textureIdx1_), textureIdx2(textureIdx2_) {}

	bool getNumberByHash(const Queryable& spell, const Queryable& player,
		uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const;

	bool getNumberByHash(const Queryable& player, uint16_t propHash, LevelObjValue& value) const override;
	bool getNumberByHash(const Queryable& player, uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const;

	bool getNumber(const std::string_view prop, Number32& value) const override;

	bool getProperty(const Queryable& spell, const Queryable& player, uint16_t propHash, const std::string_view prop, Variable& var) const;

	bool getProperty(const std::string_view prop, Variable& var) const override;

	bool getTexture(uint32_t textureNumber, TextureInfo& ti) const override;

	void setTexturePack1(const std::shared_ptr<TexturePack>& texturePack_) noexcept { texturePack1 = texturePack_; }
	void setTexturePack2(const std::shared_ptr<TexturePack>& texturePack_) noexcept { texturePack2 = texturePack_; }

	void setTextureIndex1(uint32_t idx_) noexcept { textureIdx1 = idx_; }
	void setTextureIndex2(uint32_t idx_) noexcept { textureIdx2 = idx_; }

	bool getTexture1(TextureInfo& ti) const { return texturePack1->get(textureIdx1, ti); }
	bool getTexture2(TextureInfo& ti) const { return texturePack2->get(textureIdx2, ti); }

	bool hasIntByHash(uint16_t propHash) const noexcept;

	LevelObjValue getIntByHash(uint16_t propHash) const;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;

	bool setNumberByHash(uint16_t propHash, const Number32& value) override;

	bool setIntByHash(uint16_t propHash, LevelObjValue value);

	auto& Name() const noexcept { return name; }
	auto& SpellType() const noexcept { return spellType; }

	void Name(const std::string_view name_) { name = name_; }
	void SpellType(const std::string_view type_) { spellType = type_; }

	void setDescription(size_t idx, Classifier* classifier, uint16_t skipFirst);

	// if formula is empty, it clears the current formula.
	void setFormula(uint16_t nameHash, const std::string_view formula);
	void deleteFormula(uint16_t nameHash);

	LevelObjValue getLife(const Queryable& spell,
		const Queryable& player, const std::string_view minMaxNumber = {}) const;
	LevelObjValue getMana(const Queryable& spell,
		const Queryable& player, const std::string_view minMaxNumber = {}) const;
	LevelObjValue getDamage(const Queryable& spell,
		const Queryable& player, const std::string_view minMaxNumber = {}) const;
	LevelObjValue getDuration(const Queryable& spell,
		const Queryable& player, const std::string_view minMaxNumber = {}) const;
	LevelObjValue getSpeed(const Queryable& spell,
		const Queryable& player, const std::string_view minMaxNumber = {}) const;
};
