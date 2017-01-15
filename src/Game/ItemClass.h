#pragma once

#include "Actions/Action.h"
#include "CelCache.h"
#include <memory>
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
	std::string type;
	unsigned typeHash;
	std::string description;

	sf::Vector2u inventorySize;

public:
	ItemClass(const std::shared_ptr<CelTextureCacheVector>& celTextureDrop_,
		size_t celDropIdx_, const std::shared_ptr<CelTextureCache>& celTextureInventory_,
		const std::shared_ptr<CelTextureCache>& celTextureInventoryUnusable_,
		size_t celInventoryIdx_) : celTextureDrop(celTextureDrop_),
		celDropIdx(celDropIdx_), celTextureInventory(celTextureInventory_),
		celTextureInventoryUnusable(celTextureInventoryUnusable_), celInventoryIdx(celInventoryIdx_) {}

	sf::Texture& getCelDropTexture(size_t idx) const { return celTextureDrop->get(celDropIdx, idx); }
	sf::Texture& getCelDropTextureLast() const { return celTextureDrop->getLast(celDropIdx); }
	size_t getCelDropTextureSize() const { return celTextureDrop->size(celDropIdx); }

	sf::Texture& getCelInventoryTexture() const
	{
		return celTextureInventory->get(celInventoryIdx);
	}
	sf::Texture& getCelInventoryUnusableTexture() const
	{
		return celTextureInventoryUnusable->get(celInventoryIdx);
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

	const std::string& Name() const { return name; }
	const std::string& Type() const { return type; }
	unsigned TypeHash() const { return typeHash; }
	const std::string& Description() const { return description; }
	const sf::Vector2u& InventorySize() const { return inventorySize; }

	void Name(const std::string& name_) { name = name_; }
	void Type(const std::string& type_)
	{
		type = type_;
		typeHash = Utils::str2int(Utils::toLower(type_).c_str());
	}
	void Description(const std::string& description_) { description = description_; }
	void InventorySize(const sf::Vector2u& inventorySize_) { inventorySize = inventorySize_; }
};
