#pragma once

#include "BaseClass.h"
#include "Formula.h"
#include "GameProperties.h"
#include "ItemXY.h"
#include "Namer.h"
#include "Queryable.h"
#include <string>
#include "TexturePacks/TexturePack.h"
#include "Utils.h"

class ItemClass : public BaseClass
{
private:
	std::shared_ptr<TexturePack> textureDrop;
	size_t dropIdx;
	std::shared_ptr<TexturePack> textureInventory;
	size_t inventoryIdx;

	std::string name;
	std::string shortName;
	std::string type;
	std::string subType;
	uint16_t typeHash16;

	std::vector<LevelObjProperty> defaults;

	ItemXY inventorySize;

	std::shared_ptr<Namer> prefix;
	std::shared_ptr<Namer> suffix;

	std::array<std::pair<std::shared_ptr<Namer>, uint16_t>, 5> descriptions;

	std::array<std::pair<uint16_t, Formula>, 6> formulas;
	size_t formulasSize{ 0 };

	sf::Color defaultOutline{ sf::Color::Transparent };
	sf::Color defaultOutlineIgnore{ sf::Color::Transparent };

public:
	ItemClass(const std::shared_ptr<TexturePack>& textureDrop_,
		size_t dropIdx_, const std::shared_ptr<TexturePack>& textureInventory_,
		size_t inventoryIdx_) : textureDrop(textureDrop_),
		dropIdx(dropIdx_), textureInventory(textureInventory_),
		inventoryIdx(inventoryIdx_) {}

	void setDropTexturePack(const std::shared_ptr<TexturePack>& textureDrop_)
	{
		textureDrop = textureDrop_;
	}
	void setDropTextureIndex(size_t dropIdx_)
	{
		dropIdx = dropIdx_;
	}
	void setInventoryTexturePack(const std::shared_ptr<TexturePack>& textureInventory_)
	{
		textureInventory = textureInventory_;
	}
	void setInventoryTextureIndex(size_t inventoryIdx_)
	{
		inventoryIdx = inventoryIdx_;
	}

	const TexturePack* getDropTexturePack() const { return textureDrop.get(); }
	const TexturePack* getInventoryTexturePack() const { return textureInventory.get(); }
	size_t getDropTextureIndex() const { return dropIdx; }

	bool getInventoryTexture(const sf::Texture** texture, sf::IntRect& textureRect) const
	{
		return textureInventory->get(inventoryIdx, texture, textureRect);
	}

	const std::vector<LevelObjProperty> Defaults() const { return defaults; }
	void setDefault(const char* prop, LevelObjValue val);

	LevelObjValue getDefaultByHash(uint16_t propHash) const;
	LevelObjValue getDefault(const char* prop) const;
	LevelObjValue getDefault(const std::string& prop) const
	{
		return getDefault(prop.c_str());
	}

	bool getDefaultByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getDefault(const char* prop, LevelObjValue& value) const;
	bool getDefault(const std::string& prop, LevelObjValue& value) const
	{
		return getDefault(prop.c_str(), value);
	}

	const std::string& Name() const { return name; }
	const std::string& ShortName() const { return shortName; }
	const std::string& Type() const { return type; }
	const std::string& SubType() const { return subType; }
	uint16_t TypeHash16() const { return typeHash16; }
	const ItemXY& InventorySize() const { return inventorySize; }

	const sf::Color& DefaultOutline() const { return defaultOutline; }
	const sf::Color& DefaultOutlineIgnore() const { return defaultOutlineIgnore; }

	void Name(const std::string& name_) { name = name_; }
	void ShortName(const std::string& name_) { shortName = name_; }
	void Type(const std::string& type_)
	{
		type = type_;
		typeHash16 = str2int16(Utils::toLower(type_).c_str());
	}
	void SubType(const std::string& subType_) { subType = subType_; }
	void InventorySize(const ItemXY& inventorySize_) { inventorySize = inventorySize_; }

	void DefaultOutline(const sf::Color& color) { defaultOutline = color; }
	void DefaultOutlineIgnore(const sf::Color& color) { defaultOutlineIgnore = color; }

	void setPrefix(const std::shared_ptr<Namer>& namer) { prefix = namer; }
	void setSuffix(const std::shared_ptr<Namer>& namer) { suffix = namer; }

	bool getFullName(const Queryable& item, std::string& fullName) const;

	void setDescription(size_t idx, const std::shared_ptr<Namer>& namer, uint16_t skipFirst);

	bool getDescription(size_t idx, const Queryable& item, std::string& description) const;

	void setFormula(uint16_t nameHash, const Formula& formula);
	void deleteFormula(uint16_t nameHash);

	bool evalFormula(uint16_t nameHash, const LevelObject& obj, LevelObjValue& val) const;
	bool evalFormula(uint16_t nameHash, const LevelObject& objA,
		const LevelObject& objB, LevelObjValue& val) const;

	LevelObjValue evalFormula(uint16_t nameHash, LevelObject& obj) const;
	LevelObjValue evalFormula(uint16_t nameHash, LevelObject& objA,
		const LevelObject& objB) const;
};
