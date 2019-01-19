#pragma once

#include "Actions/Action.h"
#include "Classifiers.h"
#include "Formulas.h"
#include "LevelObjectClass.h"
#include "Queryable.h"
#include "Save/SaveItem.h"
#include <string>
#include "TexturePacks/TexturePack.h"
#include "Utils/FixedMap.h"
#include "Utils/Utils.h"

class Spell : public LevelObjectClass, public Queryable
{
private:
	typedef FixedMap<uint16_t, LevelObjValue, 4> SpellProperties;

	std::string name;
	std::string spellType;

	SpellProperties properties;

	std::shared_ptr<TexturePack> texturePack1;
	std::shared_ptr<TexturePack> texturePack2;
	size_t textureIdx1;
	size_t textureIdx2;

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
	using iterator = SpellProperties::iterator;
	using const_iterator = SpellProperties::const_iterator;
	using reverse_iterator = SpellProperties::reverse_iterator;
	using const_reverse_iterator = SpellProperties::const_reverse_iterator;

	iterator begin() noexcept { return properties.begin(); }
	iterator end() noexcept { return properties.end(); }
	const_iterator begin() const noexcept { return properties.begin(); }
	const_iterator end() const noexcept { return properties.end(); }
	const_iterator cbegin() const noexcept { return properties.cbegin(); }
	const_iterator cend() const noexcept { return properties.cend(); }
	reverse_iterator rbegin() noexcept { return properties.rbegin(); }
	reverse_iterator rend() noexcept { return properties.rend(); }
	const_reverse_iterator rbegin() const noexcept { return properties.rbegin(); }
	const_reverse_iterator rend() const noexcept { return properties.rend(); }
	const_reverse_iterator crbegin() const noexcept { return properties.crbegin(); }
	const_reverse_iterator crend() const noexcept { return properties.crend(); }

	Spell(const std::shared_ptr<TexturePack>& texturePack1_,
		const std::shared_ptr<TexturePack>& texturePack2_,
		size_t textureIdx1_, size_t textureIdx2_) : texturePack1(texturePack1_),
		texturePack2(texturePack2_), textureIdx1(textureIdx1_), textureIdx2(textureIdx2_) {}

	bool getNumberPropByHash(const Queryable& spell, const Queryable& player,
		uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const;

	bool getNumberPropByHash(const Queryable& player,
		uint16_t propHash, LevelObjValue& value) const;
	bool getNumberPropByHash(const Queryable& player, uint16_t propHash,
		const std::string_view minMaxNumber, LevelObjValue& value) const;

	virtual bool getNumberProp(const std::string_view prop, Number32& value) const;

	bool getProperty(const Queryable& spell, const Queryable& player,
		uint16_t propHash, const std::string_view prop, Variable& var) const;

	virtual bool getProperty(const std::string_view prop, Variable& var) const;

	virtual bool getTexture(size_t textureNumber, TextureInfo& ti) const;

	void setTexturePack1(const std::shared_ptr<TexturePack>& texturePack_) noexcept
	{
		texturePack1 = texturePack_;
	}
	void setTexturePack2(const std::shared_ptr<TexturePack>& texturePack_) noexcept
	{
		texturePack2 = texturePack_;
	}

	void setTextureIndex1(size_t idx_) noexcept { textureIdx1 = idx_; }
	void setTextureIndex2(size_t idx_) noexcept { textureIdx2 = idx_; }

	bool getTexture1(TextureInfo& ti) const
	{
		return texturePack1->get(textureIdx1, ti);
	}
	bool getTexture2(TextureInfo& ti) const
	{
		return texturePack2->get(textureIdx2, ti);
	}

	bool hasIntByHash(uint16_t propHash) const noexcept;
	bool hasInt(const std::string_view prop) const noexcept;

	LevelObjValue getIntByHash(uint16_t propHash) const;
	LevelObjValue getInt(const std::string_view prop) const;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getInt(const std::string_view prop, LevelObjValue& value) const;
	void setIntByHash(uint16_t propHash, LevelObjValue value);
	void setInt(const std::string_view prop, LevelObjValue value);

	const std::string& Name() const noexcept { return name; }
	const std::string& SpellType() const noexcept { return spellType; }

	void Name(const std::string name_) { name = name_; }
	void SpellType(const std::string type_)
	{
		spellType = type_;
	}

	void setDescription(size_t idx, Classifier* classifier, uint16_t skipFirst);

	// if formula is empty, it clears the current formula.
	void setFormula(uint16_t nameHash, const std::string_view formula);

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

struct SpellInstance : public Queryable
{
	Spell* spell{ nullptr };
	const Queryable* spellOwner{ nullptr };
	LevelObjValue spellLevel{ 0 };

	SpellInstance(Spell* spell_, const Queryable* spellOwner_, LevelObjValue spellLevel_) :
		spell(spell_), spellOwner(spellOwner_), spellLevel(spellLevel_) {}

	bool getNumberPropByHash(const Queryable& player,
		uint16_t propHash, LevelObjValue& value) const;
	bool getNumberPropByHash(const Queryable& player, uint16_t propHash,
		const std::string_view minMaxNumber, LevelObjValue& value) const;

	virtual bool getNumberProp(const std::string_view prop, Number32& value) const;
	virtual bool getProperty(const std::string_view prop, Variable& var) const;
	virtual bool getTexture(size_t textureNumber, TextureInfo& ti) const;
};
