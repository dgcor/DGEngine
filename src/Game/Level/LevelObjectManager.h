#pragma once

#include "Game/Classifier.h"
#include "Game/LevelObject/LevelObject.h"
#include "Game/LevelObject/LevelObjectClass.h"
#include "LevelMap.h"
#include "Utils/UnorderedStringMap.h"

class Player;

class LevelObjectManager
{
private:
	friend class Level;
	friend class LevelItem;

	std::vector<std::shared_ptr<LevelObject>> levelObjects;
	UnorderedStringMap<std::shared_ptr<LevelObject>> levelObjectIds;

	std::weak_ptr<LevelObject> clickedObject;
	std::weak_ptr<LevelObject> hoverObject;
	std::weak_ptr<Player> currentPlayer;

	UnorderedStringMap<std::unique_ptr<Classifier>> classifiers;
	UnorderedStringMap<std::unique_ptr<LevelObjectClass>> levelObjectClasses;

	void add(LevelMap& map, std::shared_ptr<LevelObject> obj, const PairFloat& mapCoord);

	// clears the clickedObject, hoverObject, currentPlayer if they're pointing to the given object.
	void clearCache(const LevelObject* obj) noexcept;

	// Removes level object from level. Object still needs to be deleted from map.
	// Returns the removed object.
	template <class T>
	std::shared_ptr<T> remove(const LevelObject* obj)
	{
		std::shared_ptr<T> oldObj;
		for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
		{
			if (it->get() == obj)
			{
				oldObj = std::move(std::dynamic_pointer_cast<T>(*it));
				if (oldObj->getId().empty() == false)
				{
					levelObjectIds.erase(oldObj->getId());
				}
				levelObjects.erase(it);
				break;
			}
		}
		clearCache(obj);
		return oldObj;
	}

	template <class T>
	void clearClasses()
	{
		for (auto it = levelObjectClasses.begin(); it != levelObjectClasses.end();)
		{
			if (dynamic_cast<T*>(it->second.get()) != nullptr)
			{
				bool classBeingUsed = false;
				for (const auto& obj : levelObjects)
				{
					if (obj->getBaseClass() == it->second.get())
					{
						classBeingUsed = true;
						break;
					}
				}
				if (classBeingUsed == false)
				{
					it = levelObjectClasses.erase(it);
					continue;
				}
			}
			++it;
		}
	}

public:
	auto& Objects() const { return levelObjects; }
	auto& ObjectIds() const { return levelObjectIds; }
	auto& Classes() const { return levelObjectClasses; }

	auto hasClickedObject() const noexcept { return clickedObject.expired() == false; }
	auto ClickedObject() const noexcept { return clickedObject.lock(); }
	void ClickedObject(std::weak_ptr<LevelObject> object) noexcept { clickedObject = object; }
	void clearClickedObject() noexcept { clickedObject.reset(); }

	auto hasHoverObject() const noexcept { return hoverObject.expired() == false; }
	auto HoverObject() const noexcept { return hoverObject.lock(); }
	void HoverObject(std::weak_ptr<LevelObject> object) noexcept { hoverObject = object; }
	void clearHoverObject() noexcept { hoverObject.reset(); }

	auto hasCurrentPlayer() const noexcept { return currentPlayer.expired() == false; }
	auto CurrentPlayer() const noexcept { return currentPlayer.lock(); }
	void clearCurrentPlayer() noexcept { currentPlayer.reset(); }

	// LevelObjects

	void updatePositions(LevelMap& map);

	void add(LevelMap& map, std::shared_ptr<LevelObject> obj);

	// Deletes level object by id. If id is empty, no action is performed.
	void deleteById(LevelMap& map, const std::string_view id);

	// Deletes level object by class id. If id is empty, no action is performed.
	void deleteByClass(LevelMap& map, const std::string_view classId);

	void clearAll(LevelMap& map);

	template <class T>
	void clear(LevelMap& map)
	{
		auto it = levelObjects.begin();
		while (it != levelObjects.end())
		{
			if (dynamic_cast<T*>(it->get()) != nullptr)
			{
				const auto& mapPos = (*it)->MapPosition();
				if (map.isMapCoordValid(mapPos) == true)
				{
					map[mapPos].removeObject(it->get());
				}
				if ((*it)->getId().empty() == false)
				{
					levelObjectIds.erase((*it)->getId());
				}
				clearCache(it->get());
				it = levelObjects.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	template <class T>
	void clear(LevelMap& map, const std::vector<std::string>& excludeIds)
	{
		auto it = levelObjects.begin();
		while (it != levelObjects.end())
		{
			if (dynamic_cast<T*>(it->get()) != nullptr
				&& std::find(excludeIds.begin(), excludeIds.end(),
				(*it)->getId()) == excludeIds.end())
			{
				const auto& mapPos = (*it)->MapPosition();
				if (map.isMapCoordValid(mapPos) == true)
				{
					map[mapPos].removeObject(it->get());
				}
				if ((*it)->getId().empty() == false)
				{
					levelObjectIds.erase((*it)->getId());
				}
				it = levelObjects.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	template <class T>
	T* get(const std::string_view id) const
	{
		return dynamic_cast<T*>(get(id));
	}

	template <class T>
	std::shared_ptr<T> getSharedPtr(const std::string_view id) const
	{
		return std::dynamic_pointer_cast<T>(getSharedPtr(id));
	}

	// get level object by id
	LevelObject* get(const std::string_view id) const;

	// get level object by id
	std::shared_ptr<LevelObject> getSharedPtr(const std::string_view id) const;

	// get level object by class id
	LevelObject* getByClass(const std::string_view classId) const noexcept;

	// get level object by internal id (clickedObject, hoverObject, etc) or by id
	std::shared_ptr<LevelObject> getByQueryId(const std::string_view id) const;

	// Classifiers

	void addClassifier(const std::string_view key, std::unique_ptr<Classifier> obj)
	{
		classifiers.insert(std::make_pair(key, std::move(obj)));
	}

	Classifier* getClassifier(const std::string_view key) const
	{
		auto it = classifiers.find(key);
		if (it != classifiers.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	// Classes

	bool hasClass(const std::string_view key) const
	{
		return levelObjectClasses.find(key) != levelObjectClasses.end();
	}

	void addClass(const std::string_view key, std::unique_ptr<LevelObjectClass> obj)
	{
		levelObjectClasses.insert(std::make_pair(key, std::move(obj)));
	}

	template <class T>
	T* getClass(const std::string_view key) const
	{
		auto it = levelObjectClasses.find(key);
		if (it != levelObjectClasses.end())
		{
			return dynamic_cast<T*>(it->second.get());
		}
		return nullptr;
	}
};
