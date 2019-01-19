#include "PlayerClass.h"
#include "Player.h"
#include "Utils/Utils.h"

TexturePack* PlayerClass::getTexturePack(size_t idx) const
{
	if (idx < textures.size())
	{
		return textures[idx].first.get();
	}
	else if (textures.empty() == false)
	{
		return textures.front().first.get();
	}
	return nullptr;
}

void PlayerClass::getTextureAnimationRange(size_t idx,
	PlayerAnimation animation, BaseAnimation& baseAnim) const
{
	const std::pair<std::shared_ptr<TexturePack>, Ranges>* tex = nullptr;
	if (idx < textures.size())
	{
		tex = &textures[idx];
	}
	else if (textures.empty() == false)
	{
		tex = &textures.front();
	}
	if (tex != nullptr)
	{
		size_t animIdx = 0;
		if (animation < PlayerAnimation::Size)
		{
			animIdx = animationIndexes[(size_t)animation];
		}
		if (animIdx < tex->second.size())
		{
			baseAnim.textureIndexRange = tex->second[animIdx].range;
			baseAnim.animType = tex->second[animIdx].animType;
			baseAnim.backDirection = false;
		}
		else
		{
			baseAnim.textureIndexRange = std::make_pair(0, tex->first->size() - 1);
			baseAnim.animType = AnimationType::Looped;
			baseAnim.backDirection = false;
		}
		return;
	}
	baseAnim.textureIndexRange = std::make_pair(0, 0);
	baseAnim.animType = AnimationType::PlayOnce;
	baseAnim.backDirection = false;
}

AnimationSpeed PlayerClass::getSpeed(PlayerAnimation animation) const noexcept
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

const sf::SoundBuffer* PlayerClass::getSound(size_t idx) const noexcept
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

void PlayerClass::setSound(size_t idx, const sf::SoundBuffer& snd) noexcept
{
	if (idx < sounds.size())
	{
		sounds[idx] = &snd;
	}
}

void PlayerClass::setFormula(uint16_t nameHash, const std::string_view formula)
{
	if (nameHash == str2int16(""))
	{
		return;
	}
	switch (nameHash)
	{
	case str2int16("life"):
	{
		formulas[0] = formula;
		break;
	}
	case str2int16("mana"):
	{
		formulas[1] = formula;
		break;
	}
	case str2int16("armor"):
	{
		formulas[2] = formula;
		break;
	}
	case str2int16("toHit"):
	{
		formulas[3] = formula;
		break;
	}
	case str2int16("damage"):
	{
		formulas[4] = formula;
		break;
	}
	case str2int16("resistMagic"):
	{
		formulas[5] = formula;
		break;
	}
	case str2int16("resistFire"):
	{
		formulas[6] = formula;
		break;
	}
	case str2int16("resistLightning"):
	{
		formulas[7] = formula;
		break;
	}
	default:
		break;
	}
}
