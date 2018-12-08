#include "LevelCell.h"

void LevelCell::clearLights(uint8_t defaultLight_) noexcept
{
	defaultLight = defaultLight_;
	currentLight = 0;
	lights.clear();
}

void LevelCell::addLight(uint8_t light_) noexcept
{
	if (light_ == 0)
	{
		return;
	}
	if (currentLight == 0)
	{
		currentLight = light_;
		return;
	}
	uint8_t newLight;
	if (light_ > currentLight)
	{
		newLight = currentLight;
		currentLight = light_;
	}
	else
	{
		newLight = light_;
	}
	lights.push_back(newLight);
	std::sort(lights.begin(), lights.end());
}

void LevelCell::subtractLight(uint8_t light_) noexcept
{
	if (light_ == 0 ||
		currentLight == 0)
	{
		return;
	}
	if (light_ >= currentLight)
	{
		if (lights.empty() == true)
		{
			currentLight = 0;
			return;
		}
		currentLight = lights.back();
		lights.pop_back();
	}
	else
	{
		auto it = std::find_if(lights.begin(), lights.end(),
			[&light_](const auto& l) -> bool { return l >= light_; }
		);
		if (it != lights.end())
		{
			lights.erase(it);
		}
	}
}

void LevelCell::setTileIndex(int16_t tileIndex_) noexcept
{
	for (size_t i = 0; i < tileIndexes.size() - 1; i++)
	{
		tileIndexes[i] = tileIndex_;
	}
}

bool LevelCell::PassableIgnoreObject(const LevelObject* ignoreObj) const
{
	if (PassableIgnoreObject() == false)
	{
		return false;
	}
	for (const auto obj : objects)
	{
		if (obj == ignoreObj)
		{
			continue;
		}
		if (obj->Passable() == false)
		{
			return false;
		}
	}
	return true;
}

bool LevelCell::Passable() const
{
	if (PassableIgnoreObject() == false)
	{
		return false;
	}
	for (const auto obj : objects)
	{
		if (obj->Passable() == false)
		{
			return false;
		}
	}
	return true;
}

LevelObject* LevelCell::back() const
{
	if (objects.empty() == false)
	{
		return objects.back();
	}
	return nullptr;
}

LevelObject* LevelCell::front() const
{
	if (objects.empty() == false)
	{
		return objects.front();
	}
	return nullptr;
}

void LevelCell::addFront(LevelObject* obj)
{
	if (std::find(objects.begin(), objects.end(), obj) == objects.end())
	{
		objects.insert(objects.begin(), obj);
	}
}

void LevelCell::addBack(LevelObject* obj)
{
	if (std::find(objects.begin(), objects.end(), obj) == objects.end())
	{
		objects.push_back(obj);
	}
}

bool LevelCell::removeObject(const LevelObject* obj)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (*it == obj)
		{
			objects.erase(it);
			return true;
		}
	}
	return false;
}
