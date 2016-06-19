#include "Player.h"
#include "Game.h"
#include "GameUtils.h"
#include "Level.h"
#include "Utils.h"

using Utils::str2int;

void Player::calculatePosition(const Level& level, const sf::Vector2u& texSize)
{
	auto drawPos = level.Map().getCoords(position);
	drawPos.x += (float)(level.getLevelX_() - ((int)texSize.x / 2)) + 32;
	drawPos.y += (float)(level.getLevelY_() + 224 - ((int)texSize.y - 32));
	sprite.setPosition(drawPos);
}

void Player::calculateFrameRange(const sf::Vector2i& oldPos, const sf::Vector2i& newPos)
{
	//if (frames == 8)
	//{
	//	if (oldPos.x < newPos.x)
	//	{

	//	}
	//}
}

void Player::update(Game& game, Level& level)
{
	auto rect = sprite.getGlobalBounds();
	if (rect.contains(level.MousePosition()))
	{
		if (game.getMouseButton() == sf::Mouse::Left)
		{
			if (game.wasMouseClicked() == true)
			{
				game.clearMouseClicked();
				if (clickAction != nullptr)
				{
					game.Events().addBack(clickAction);
				}
			}
		}
	}
	if (rect.width > 0 && rect.height > 0)
	{
		calculatePosition(level, sf::Vector2u(rect.width, rect.height));
	}

	if (frameRange.first > frameRange.second)
	{
		return;
	}

	// add delta time
	m_currentTime += game.getElapsedTime();

	// if current time is bigger then the frame time advance one frame
	if (m_currentTime >= m_frameTime)
	{
		// reset time, but keep the remainder
		m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

		currentFrame++;
		if (currentFrame < frameRange.first || currentFrame >= frameRange.second)
		{
			currentFrame = frameRange.first;
		}

		if (currentFrame < celTexture->size(celIdx))
		{
			sprite.setTexture(celTexture->get(celIdx, currentFrame), true);
		}
	}
}

Variable Player::getProperty(const std::string& prop) const
{
	if (prop.size() > 1)
	{
		switch (str2int(prop.c_str()))
		{
		case str2int("name"):
			return Variable(name);
		case str2int("class"):
			return Variable(class_);
		case str2int("level"):
			return Variable((int64_t)level);
		case str2int("experience"):
			return Variable((int64_t)experience);
		case str2int("expNextLevel"):
			return Variable((int64_t)expNextLevel);
		case str2int("points"):
			return Variable((int64_t)points);
		case str2int("gold"):
			return Variable((int64_t)gold);
		case str2int("strengthBase"):
			return Variable((int64_t)strengthBase);
		case str2int("strengthNow"):
			return Variable((int64_t)strengthNow);
		case str2int("magicBase"):
			return Variable((int64_t)magicBase);
		case str2int("magicNow"):
			return Variable((int64_t)magicNow);
		case str2int("dexterityBase"):
			return Variable((int64_t)dexterityBase);
		case str2int("dexterityNow"):
			return Variable((int64_t)dexterityNow);
		case str2int("vitalityBase"):
			return Variable((int64_t)vitalityBase);
		case str2int("vitalityNow"):
			return Variable((int64_t)vitalityNow);
		case str2int("lifeBase"):
			return Variable((int64_t)lifeBase);
		case str2int("lifeNow"):
			return Variable((int64_t)lifeNow);
		case str2int("manaBase"):
			return Variable((int64_t)manaBase);
		case str2int("manaNow"):
			return Variable((int64_t)manaNow);
		case str2int("armorClass"):
			return Variable((int64_t)armorClass);
		case str2int("toHit"):
			return Variable((int64_t)toHit);
		case str2int("damageMin"):
			return Variable((int64_t)damageMin);
		case str2int("damageMax"):
			return Variable((int64_t)damageMax);
		case str2int("resistMagic"):
			return Variable((int64_t)resistMagic);
		case str2int("resistFire"):
			return Variable((int64_t)resistFire);
		case str2int("resistLightning"):
			return Variable((int64_t)resistLightning);
		default:
			break;
		}
	}
	return Variable();
}
