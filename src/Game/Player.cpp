#include "Player.h"
#include "Game.h"
#include "GameUtils.h"
#include "Level.h"
#include "Utils.h"

using Utils::str2int;

void Player::calculatePosition(Level& level, const sf::Vector2u& texSize)
{
	if (walkPath.empty() == true)
	{
		setStatus(PlayerStatus::Stand1);
	}
	else
	{
		setStatus(PlayerStatus::Walk1);
	}

	if (walkPath.empty() == false)
	{
		setDirection(getPlayerDirection(mapPosition, walkPath.front()));
		level.Map()[mapPosition.x][mapPosition.y].drawable = nullptr;
		mapPosition = walkPath.front();
		walkPath.pop();
		level.Map()[mapPosition.x][mapPosition.y].drawable = this;
	}

	auto drawPos = level.Map().getCoords(mapPosition);
	drawPos.x += (float)(level.getLevelX() - ((int)texSize.x / 2)) + 32;
	drawPos.y += (float)(level.getLevelY() + 224 - ((int)texSize.y - 32));
	sprite.setPosition(drawPos);
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

	if (celTexture == nullptr
		|| frameRange.first > frameRange.second)
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

bool Player::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() > 1)
	{
		switch (str2int(prop.c_str()))
		{
		case str2int("name"):
			var = Variable(name);
			break;
		case str2int("class"):
			var = Variable(class_->Name());
			break;
		case str2int("level"):
			var = Variable((int64_t)level);
			break;
		case str2int("experience"):
			var = Variable((int64_t)experience);
			break;
		case str2int("expNextLevel"):
			var = Variable((int64_t)expNextLevel);
			break;
		case str2int("points"):
			var = Variable((int64_t)points);
			break;
		case str2int("gold"):
			var = Variable((int64_t)gold);
			break;
		case str2int("strengthBase"):
			var = Variable((int64_t)strengthBase);
			break;
		case str2int("strengthNow"):
			var = Variable((int64_t)strengthNow);
			break;
		case str2int("magicBase"):
			var = Variable((int64_t)magicBase);
			break;
		case str2int("magicNow"):
			var = Variable((int64_t)magicNow);
			break;
		case str2int("dexterityBase"):
			var = Variable((int64_t)dexterityBase);
			break;
		case str2int("dexterityNow"):
			var = Variable((int64_t)dexterityNow);
			break;
		case str2int("vitalityBase"):
			var = Variable((int64_t)vitalityBase);
			break;
		case str2int("vitalityNow"):
			var = Variable((int64_t)vitalityNow);
			break;
		case str2int("lifeBase"):
			var = Variable((int64_t)lifeBase);
			break;
		case str2int("lifeNow"):
			var = Variable((int64_t)lifeNow);
			break;
		case str2int("manaBase"):
			var = Variable((int64_t)manaBase);
			break;
		case str2int("manaNow"):
			var = Variable((int64_t)manaNow);
			break;
		case str2int("armorClass"):
			var = Variable((int64_t)armorClass);
			break;
		case str2int("toHit"):
			var = Variable((int64_t)toHit);
			break;
		case str2int("damageMin"):
			var = Variable((int64_t)damageMin);
			break;
		case str2int("damageMax"):
			var = Variable((int64_t)damageMax);
			break;
		case str2int("resistMagic"):
			var = Variable((int64_t)resistMagic);
			break;
		case str2int("resistFire"):
			var = Variable((int64_t)resistFire);
			break;
		case str2int("resistLightning"):
			var = Variable((int64_t)resistLightning);
			break;
		default:
			return false;
		}
		return true;
	}
	return false;
}
