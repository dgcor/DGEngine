#pragma once

#include "BaseClass.h"
#include "Formula.h"
#include "GameProperties.h"
#include "ItemXY.h"
#include "Namer.h"
#include "Queryable.h"
#include <string>
#include "TexturePacks/TexturePack.h"
#include "Utils/Utils.h"

class ItemClass : public BaseClass
{
private:
	std::shared_ptr<TexturePack> textureDrop;
	std::shared_ptr<TexturePack> textureInventory;
	std::pair<size_t, size_t> dropTextureIndexRange;
	size_t inventoryIdx;

	std::string name;
	std::string shortName;
	std::string type;
	std::string subType;
	uint16_t typeHash16{ 0 };

	std::vector<LevelObjProperty> defaults;

	ItemXY inventorySize;

	Namer* prefix{ nullptr };
	Namer* suffix{ nullptr };

	std::array<std::pair<Namer*, uint16_t>, 5> descriptions;

	std::array<std::pair<uint16_t, Formula>, 6> formulas;
	size_t formulasSize{ 0 };

	sf::Time defaultAnimationSpeed{ sf::milliseconds(40) };
	sf::Color defaultOutline{ sf::Color::Transparent };
	sf::Color defaultOutlineIgnore{ sf::Color::Transparent };

public:
	ItemClass(const std::shared_ptr<TexturePack>& textureDrop_,
		const std::shared_ptr<TexturePack>& textureInventory_,
		size_t inventoryIdx_);

	void setDropTexturePack(const std::shared_ptr<TexturePack>& textureDrop_) noexcept;
	const std::pair<size_t, size_t>& getDropTextureIndexRange() const noexcept
	{
		return dropTextureIndexRange;
	}
	void setDropTextureIndexRange(const std::pair<size_t, size_t>& textureIndexRange) noexcept
	{
		dropTextureIndexRange = textureIndexRange;
	}

	void setInventoryTexturePack(const std::shared_ptr<TexturePack>& textureInventory_) noexcept
	{
		textureInventory = textureInventory_;
	}
	void setInventoryTextureIndex(size_t inventoryIdx_) noexcept
	{
		inventoryIdx = inventoryIdx_;
	}

	const TexturePack* getDropTexturePack() const noexcept { return textureDrop.get(); }
	const TexturePack* getInventoryTexturePack() const noexcept { return textureInventory.get(); }

	bool getInventoryTexture(TextureInfo& ti) const
	{
		return textureInventory->get(inventoryIdx, ti);
	}

	const std::vector<LevelObjProperty>& Defaults() const noexcept { return defaults; }
	void setDefault(const char* prop, LevelObjValue val);

	LevelObjValue getDefaultByHash(uint16_t propHash) const noexcept;
	LevelObjValue getDefault(const char* prop) const noexcept;
	LevelObjValue getDefault(const std::string& prop) const noexcept
	{
		return getDefault(prop.c_str());
	}

	bool getDefaultByHash(uint16_t propHash, LevelObjValue& value) const noexcept;
	bool getDefault(const char* prop, LevelObjValue& value) const noexcept;
	bool getDefault(const std::string& prop, LevelObjValue& value) const noexcept
	{
		return getDefault(prop.c_str(), value);
	}

	const std::string& Name() const noexcept { return name; }
	const std::string& ShortName() const noexcept { return shortName; }
	const std::string& Type() const noexcept { return type; }
	const std::string& SubType() const noexcept { return subType; }
	uint16_t TypeHash16() const noexcept { return typeHash16; }
	const ItemXY& InventorySize() const noexcept { return inventorySize; }

	const sf::Time& DefaultAnimationSpeed() const noexcept { return defaultAnimationSpeed; }
	const sf::Color& DefaultOutline() const noexcept { return defaultOutline; }
	const sf::Color& DefaultOutlineIgnore() const noexcept { return defaultOutlineIgnore; }

	void Name(const std::string name_) { name = name_; }
	void ShortName(const std::string name_) { shortName = name_; }
	void Type(const std::string type_)
	{
		type = type_;
		typeHash16 = str2int16(Utils::toLower(type_).c_str());
	}
	void SubType(const std::string subType_) { subType = subType_; }
	void InventorySize(const ItemXY inventorySize_) noexcept { inventorySize = inventorySize_; }

	void DefaultAnimationSpeed(const sf::Time time) noexcept { defaultAnimationSpeed = time; }
	void DefaultOutline(const sf::Color color) noexcept { defaultOutline = color; }
	void DefaultOutlineIgnore(const sf::Color color) noexcept { defaultOutlineIgnore = color; }

	void setPrefix(Namer* namer) noexcept { prefix = namer; }
	void setSuffix(Namer* namer) noexcept { suffix = namer; }

	bool getFullName(const Queryable& item, std::string& fullName) const;

	void setDescription(size_t idx, Namer* namer, uint16_t skipFirst);

	bool getDescription(size_t idx, const Queryable& item, std::string& description) const;

	void setFormula(uint16_t nameHash, const Formula& formula);
	void deleteFormula(uint16_t nameHash);

	bool evalFormula(uint16_t nameHash, const LevelObject& obj, LevelObjValue& val) const;
	bool evalFormula(uint16_t nameHash, const LevelObject& objA,
		const LevelObject& objB, LevelObjValue& val) const;

	LevelObjValue evalFormula(uint16_t nameHash, const LevelObject& obj) const;
	LevelObjValue evalFormula(uint16_t nameHash, const LevelObject& objA,
		const LevelObject& objB) const;
};
