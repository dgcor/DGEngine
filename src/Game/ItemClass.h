#pragma once

#include "Classifiers.h"
#include "Formula.h"
#include "GameProperties.h"
#include "LevelObjectClassDefaults.h"
#include "Spell.h"
#include <string>
#include "TexturePacks/TexturePack.h"
#include "Utils/FixedMap.h"
#include "Utils/PairXY.h"

class ItemClass : public LevelObjectClassDefaults<LevelObjValue>
{
private:
	std::shared_ptr<TexturePack> textureDrop;
	std::shared_ptr<TexturePack> textureInventory;
	std::pair<uint32_t, uint32_t> dropTextureIndexRange;
	uint32_t inventoryIdx;

	std::string name;
	std::string shortName;
	std::string type;
	std::string subType;
	uint16_t typeHash16{ 0 };

	PairUInt8 inventorySize;

	Classifiers<12> classifiers;

	FixedMap<uint16_t, Formula, 6> formulas;

	SpellInstance spell;

	sf::Time animationSpeed{ sf::milliseconds(40) };
	sf::Color outline{ sf::Color::Transparent };
	sf::Color outlineIgnore{ sf::Color::Transparent };

	static constexpr size_t InventoryTextureClassifier = 0;
	static constexpr size_t PrefixClassifier = 1;
	static constexpr size_t SuffixClassifier = 2;
	static constexpr size_t PricePrefix1Classifier = 3;
	static constexpr size_t PricePrefix2Classifier = 4;
	static constexpr size_t PriceSuffix1Classifier = 5;
	static constexpr size_t PriceSuffix2Classifier = 6;
	static constexpr size_t DescriptionClassifier = 7;

	LevelObjValue getClassifierNumOrFormula(size_t idx, const Queryable& item) const;

public:
	ItemClass(const std::shared_ptr<TexturePack>& textureDrop_,
		const std::shared_ptr<TexturePack>& textureInventory_,
		uint32_t inventoryIdx_);

	void setDropTexturePack(const std::shared_ptr<TexturePack>& textureDrop_) noexcept;
	const std::pair<uint32_t, uint32_t>& getDropTextureIndexRange() const noexcept
	{
		return dropTextureIndexRange;
	}
	void setDropTextureIndexRange(const std::pair<uint32_t, uint32_t>& textureIndexRange) noexcept
	{
		dropTextureIndexRange = textureIndexRange;
	}

	void setInventoryTexturePack(const std::shared_ptr<TexturePack>& textureInventory_) noexcept
	{
		textureInventory = textureInventory_;
	}
	void setInventoryTextureIndex(uint32_t inventoryIdx_) noexcept
	{
		inventoryIdx = inventoryIdx_;
	}

	const std::shared_ptr<TexturePack>& getDropTexturePack() const noexcept { return textureDrop; }
	const TexturePack* getInventoryTexturePack() const noexcept { return textureInventory.get(); }

	bool getInventoryTexture(const Queryable& item, TextureInfo& ti) const;

	const std::string& Name() const noexcept { return name; }
	const std::string& ShortName() const noexcept { return shortName; }
	const std::string& Type() const noexcept { return type; }
	const std::string& SubType() const noexcept { return subType; }
	uint16_t TypeHash16() const noexcept { return typeHash16; }
	const PairUInt8& InventorySize() const noexcept { return inventorySize; }

	const sf::Time& AnimationSpeed() const noexcept { return animationSpeed; }
	const sf::Color& Outline() const noexcept { return outline; }
	const sf::Color& OutlineIgnore() const noexcept { return outlineIgnore; }

	void setInventoryTexture(Classifier* classifier);
	void setPrefix(Classifier* classifier);
	void setSuffix(Classifier* classifier);

	void Name(const std::string_view name_) { name = name_; }
	void ShortName(const std::string_view name_) { shortName = name_; }
	void Type(const std::string_view type_)
	{
		type = type_;
		typeHash16 = str2int16(Utils::toLower(type_));
	}
	void SubType(const std::string_view subType_) { subType = subType_; }
	void InventorySize(const PairUInt8 inventorySize_) noexcept { inventorySize = inventorySize_; }

	void AnimationSpeed(const sf::Time time) noexcept { animationSpeed = time; }
	void Outline(const sf::Color color) noexcept { outline = color; }
	void OutlineIgnore(const sf::Color color) noexcept { outlineIgnore = color; }

	bool getFullName(const Queryable& item, std::string& fullName) const;

	LevelObjValue getPricePrefix1(const Queryable& item) const;
	LevelObjValue getPricePrefix2(const Queryable& item) const;
	LevelObjValue getPriceSuffix1(const Queryable& item) const;
	LevelObjValue getPriceSuffix2(const Queryable& item) const;

	void setPricePrefix1(Classifier* classifier);
	void setPricePrefix2(Classifier* classifier);
	void setPriceSuffix1(Classifier* classifier);
	void setPriceSuffix2(Classifier* classifier);

	void setDescription(size_t idx, Classifier* classifier, uint16_t skipFirst);

	bool getDescription(size_t idx, const Queryable& item, std::string& description) const;

	bool hasSpell() const noexcept { return spell.spell != nullptr; }
	const SpellInstance* getSpell() const noexcept;
	void setSpell(Spell* obj);

	void setFormula(uint16_t nameHash, const std::string_view formula);
	void deleteFormula(uint16_t nameHash);

	bool evalFormula(uint16_t nameHash, const Queryable& item,
		const Queryable& itemOwner, LevelObjValue& val,
		const std::string_view minMaxNumber = {}) const;

	LevelObjValue evalFormula(uint16_t nameHash, const Queryable& item,
		const Queryable& itemOwner, const std::string_view minMaxNumber = {}) const;
};
