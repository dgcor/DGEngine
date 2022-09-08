#include "LevelObjectManager.h"
#include "Game/Player/Player.h"

void LevelObjectManager::updatePositions(LevelMap& map)
{
	for (const auto& obj : levelObjects)
	{
		const auto& mapPosition = obj->MapPosition();
		if (map.isMapCoordValid(mapPosition) == true)
		{
			obj->MapPosition(map, mapPosition);
		}
	}
}

void LevelObjectManager::add(LevelMap& map, std::shared_ptr<LevelObject> obj)
{
	auto& mapPos = obj->MapPosition();
	add(map, std::move(obj), mapPos);
}

void LevelObjectManager::add(LevelMap& map, std::shared_ptr<LevelObject> obj, const PairFloat& mapCoord)
{
	if (obj->getId().empty() == false)
	{
		if (levelObjectIds.find(obj->getId()) != levelObjectIds.end())
		{
			return;
		}
		levelObjectIds[obj->getId()] = obj;
	}
	obj->MapPosition(map, mapCoord);
	levelObjects.push_back(obj);
}

void LevelObjectManager::clearCache(const LevelObject* obj) noexcept
{
	if (clickedObject.lock().get() == obj)
	{
		clickedObject.reset();
	}
	if (hoverObject.lock().get() == obj)
	{
		hoverObject.reset();
	}
	if (currentPlayer.lock().get() == obj)
	{
		currentPlayer.reset();
	}
}

void LevelObjectManager::deleteById(LevelMap& map, const std::string_view id)
{
	if (id.empty() == true)
	{
		return;
	}
	for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
	{
		if ((*it)->getId() == id)
		{
			auto obj = it->get();
			obj->remove(map);
			clearCache(obj);
			if (obj->getId().empty() == false)
			{
				levelObjectIds.erase(obj->getId());
			}
			levelObjects.erase(it);
			break;
		}
	}
}

void LevelObjectManager::deleteByClass(LevelMap& map, const std::string_view classId)
{
	if (classId.empty() == true)
	{
		return;
	}
	for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
	{
		if ((*it)->getClassId() == classId)
		{
			auto obj = it->get();
			obj->remove(map);
			clearCache(obj);
			if (obj->getId().empty() == false)
			{
				levelObjectIds.erase(obj->getId());
			}
			levelObjects.erase(it);
			break;
		}
	}
}

void LevelObjectManager::clearAll(LevelMap& map)
{
	for (const auto& obj : levelObjects)
	{
		const auto& mapPos = obj->MapPosition();
		if (map.isMapCoordValid(mapPos) == true)
		{
			map[mapPos].removeObject(obj.get());
		}
	}
	levelObjects.clear();
	levelObjectIds.clear();
}

LevelObject* LevelObjectManager::get(const std::string_view id) const
{
	if (id.empty() == true)
	{
		return nullptr;
	}
	auto it = levelObjectIds.find(id);
	if (it != levelObjectIds.cend())
	{
		return it->second.get();
	}
	return nullptr;
}

std::shared_ptr<LevelObject> LevelObjectManager::getSharedPtr(const std::string_view id) const
{
	if (id.empty() == true)
	{
		return nullptr;
	}
	auto it = levelObjectIds.find(id);
	if (it != levelObjectIds.cend())
	{
		return it->second;
	}
	return nullptr;
}

LevelObject* LevelObjectManager::getByClass(const std::string_view classId) const noexcept
{
	if (classId.empty() == true)
	{
		return nullptr;
	}
	for (const auto& obj : levelObjects)
	{
		if (obj->getClassId() == classId)
		{
			return obj.get();
		}
	}
	return nullptr;
}

std::shared_ptr<LevelObject> LevelObjectManager::getByQueryId(const std::string_view id) const
{
	if (id.empty() == true)
	{
		return {};
	}
	switch (str2int16(id))
	{
	case str2int16("clickedObject"):
		return clickedObject.lock();
	case str2int16("currentPlayer"):
		return currentPlayer.lock();
	case str2int16("hoverObject"):
		return hoverObject.lock();
	default:
		break;
	}
	auto it = levelObjectIds.find(id);
	if (it != levelObjectIds.cend())
	{
		return it->second;
	}
	return {};
}
