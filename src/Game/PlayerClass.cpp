#include "PlayerClass.h"
#include "Player.h"

void PlayerClass::setDefault(const char* prop, const Number32& val)
{
	auto propertyHash = str2int16(prop);
	if (propertyHash == str2int16(""))
	{
		return;
	}
	for (auto& elem : defaults)
	{
		if (elem.first == propertyHash)
		{
			elem.second = val;
			return;
		}
	}
	defaults.push_back(std::make_pair(propertyHash, val));
}

AnimationSpeed PlayerClass::getSpeed(PlayerStatus status) const
{
	for (auto& elem : animationSpeeds)
	{
		if (elem.first == status)
		{
			return elem.second;
		}
	}
	return {};
}

void PlayerClass::setSpeed(PlayerStatus status, const AnimationSpeed& speed)
{
	for (auto& elem : animationSpeeds)
	{
		if (elem.first == status)
		{
			elem.second = speed;
			return;
		}
	}
	animationSpeeds.push_back(std::make_pair(status, speed));
}

LevelObjValue PlayerClass::evalFormula(size_t idx,
	const LevelObject& query, LevelObjValue default_) const
{
	if (formulas[idx].empty() == true)
	{
		return default_;
	}
	return (LevelObjValue)formulas[idx].eval(query);
}
