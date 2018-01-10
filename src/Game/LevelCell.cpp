#include "LevelCell.h"

bool LevelCell::Passable() const
{
	if (PassableIgnoreObject() == false)
	{
		return false;
	}
	for (const auto& obj : objects)
	{
		if (obj->Passable() == false)
		{
			return false;
		}
	}
	return true;
}

std::shared_ptr<LevelObject> LevelCell::back() const
{
	if (objects.empty() == false)
	{
		return objects.back();
	}
	return nullptr;
}

std::shared_ptr<LevelObject> LevelCell::front() const
{
	if (objects.empty() == false)
	{
		return objects.front();
	}
	return nullptr;
}

std::shared_ptr<LevelObject> LevelCell::getObject(LevelObject* obj) const
{
	for (const auto& object : objects)
	{
		if (object.get() == obj)
		{
			return object;
		}
	}
	return nullptr;
}

void LevelCell::addFront(const std::shared_ptr<LevelObject>& obj)
{
	objects.insert(objects.begin(), obj);
}

void LevelCell::addBack(const std::shared_ptr<LevelObject>& obj)
{
	objects.push_back(obj);
}

void LevelCell::deleteObject(LevelObject* obj)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->get() == obj)
		{
			objects.erase(it);
			return;
		}
	}
}
