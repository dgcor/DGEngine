#include "PlayerClass.h"
#include "Player.h"
#include "Utils.h"

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

AnimationSpeed PlayerClass::getSpeed(PlayerAnimation animation) const
{
	for (auto& elem : animationSpeeds)
	{
		if (elem.first == animation)
		{
			return elem.second;
		}
	}
	return {};
}

void PlayerClass::setSpeed(PlayerAnimation animation, const AnimationSpeed& speed)
{
	for (auto& elem : animationSpeeds)
	{
		if (elem.first == animation)
		{
			elem.second = speed;
			return;
		}
	}
	animationSpeeds.push_back(std::make_pair(animation, speed));
}

const sf::SoundBuffer* PlayerClass::getSound(size_t idx) const
{
	if (idx < sounds.size())
	{
		return sounds[idx];
	}
	return nullptr;
}

const sf::SoundBuffer* PlayerClass::getSound(size_t idx, size_t size) const
{
	if (size <= 1)
	{
		return getSound(idx);
	}
	auto maxIdx = idx + size - 1;
	if (maxIdx >= sounds.size() || maxIdx <= idx)
	{
		return getSound(idx);
	}
	auto rndIdx = Utils::Random::get<size_t>(idx, maxIdx);
	return sounds[rndIdx];
}

void PlayerClass::setSound(size_t idx, const sf::SoundBuffer& snd)
{
	if (idx < sounds.size())
	{
		sounds[idx] = &snd;
	}
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
