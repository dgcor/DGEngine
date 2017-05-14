#pragma once

#include "Actions/Action.h"
#include "CelCache.h"
#include "Formula.h"
#include "GameProperties.h"
#include "ItemXY.h"
#include <memory>
#include "Namer.h"
#include "Queryable.h"
#include <string>
#include "Utils.h"

class ItemClass
{
private:
	std::shared_ptr<CelTextureCacheVector> celTextureDrop;
	size_t celDropIdx;
	std::shared_ptr<CelTextureCache> celTextureInventory;
	std::shared_ptr<CelTextureCache> celTextureInventoryUnusable;
	size_t celInventoryIdx;

	std::shared_ptr<Action> action;

	std::shared_ptr<Action> actionDropLevel;
	std::shared_ptr<Action> actionPickLevel;

	std::shared_ptr<Action> actionDropInventory;
	std::shared_ptr<Action> actionPickInventory;

	std::string name;
	std::string shortName;
	std::string type;
	uint16_t typeHash16;

	std::vector<LevelObjProperty> defaults;

	ItemXY inventorySize;

	std::shared_ptr<Namer> prefix;
	std::shared_ptr<Namer> suffix;

	std::array<std::shared_ptr<Namer>, 5> descriptions;

	std::array<Formula, 4> priceFormulas;

public:
	ItemClass(const std::shared_ptr<CelTextureCacheVector>& celTextureDrop_,
		size_t celDropIdx_, const std::shared_ptr<CelTextureCache>& celTextureInventory_,
		const std::shared_ptr<CelTextureCache>& celTextureInventoryUnusable_,
		size_t celInventoryIdx_) : celTextureDrop(celTextureDrop_),
		celDropIdx(celDropIdx_), celTextureInventory(celTextureInventory_),
		celTextureInventoryUnusable(celTextureInventoryUnusable_), celInventoryIdx(celInventoryIdx_) {}

	void setCelTextureDrop(const std::shared_ptr<CelTextureCacheVector>& celTextureDrop_)
	{
		celTextureDrop = celTextureDrop_;
	}
	void setCelDropIdx(size_t celDropIdx_)
	{
		celDropIdx = celDropIdx_;
	}
	void setCelTextureInventory(const std::shared_ptr<CelTextureCache>& celTextureInventory_)
	{
		celTextureInventory = celTextureInventory_;
	}
	void setCelTextureInventoryUnusable(const std::shared_ptr<CelTextureCache>& celTextureInventoryUnusable_)
	{
		celTextureInventoryUnusable = celTextureInventoryUnusable_;
	}
	void setCelInventoryIdx(size_t celInventoryIdx_)
	{
		celInventoryIdx = celInventoryIdx_;
	}

	sf::Texture& getCelDropTexture(size_t idx) const { return celTextureDrop->get(celDropIdx, idx); }
	sf::Texture& getCelDropTextureLast() const { return celTextureDrop->getLast(celDropIdx); }
	size_t getCelDropTextureSize() const { return celTextureDrop->size(celDropIdx); }

	sf::Texture& getCelInventoryTexture(bool equipable = true) const
	{
		if (equipable == true)
		{
			return celTextureInventory->get(celInventoryIdx);
		}
		else
		{
			return celTextureInventoryUnusable->get(celInventoryIdx);
		}
	}

	const std::shared_ptr<Action>& getAction() const { return action; }

	const std::shared_ptr<Action>& getActionDropLevel() const { return actionDropLevel; }
	const std::shared_ptr<Action>& getActionPickLevel() const { return actionPickLevel; }

	const std::shared_ptr<Action>& getActionDropInventory() const { return actionDropInventory; }
	const std::shared_ptr<Action>& getActionPickInventory() const { return actionPickInventory; }

	void setAction(const std::shared_ptr<Action>& action_) { action = action_; }

	void setActionDropLevel(const std::shared_ptr<Action>& action_) { actionDropLevel = action_; }
	void setActionPickLevel(const std::shared_ptr<Action>& action_) { actionPickLevel = action_; }

	void setActionDropInventory(const std::shared_ptr<Action>& action_) { actionDropInventory = action_; }
	void setActionPickInventory(const std::shared_ptr<Action>& action_) { actionPickInventory = action_; }

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
	uint16_t TypeHash16() const { return typeHash16; }
	const ItemXY& InventorySize() const { return inventorySize; }

	void Name(const std::string& name_) { name = name_; }
	void ShortName(const std::string& name_) { shortName = name_; }
	void Type(const std::string& type_)
	{
		type = type_;
		typeHash16 = str2int16(Utils::toLower(type_).c_str());
	}
	void InventorySize(const ItemXY& inventorySize_) { inventorySize = inventorySize_; }

	void setPrefix(const std::shared_ptr<Namer>& namer) { prefix = namer; }
	void setSuffix(const std::shared_ptr<Namer>& namer) { suffix = namer; }

	bool getFullName(const Queryable& item, std::string& fullName) const;

	void setDescription(size_t idx, const std::shared_ptr<Namer>& namer);

	bool getDescription(size_t idx, const Queryable& item, std::string& description) const;

	void setPriceFormula(size_t idx, const Formula& formula);

	LevelObjValue getPrice(size_t idx, const LevelObject& item) const;
};
