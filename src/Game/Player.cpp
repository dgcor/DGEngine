#include "Player.h"
#include <cstdlib>
#include "Game.h"
#include "GameUtils.h"
#include "Level.h"
#include "Utils.h"

using Utils::str2int;

sf::Vector2f Player::getBasePosition() const
{
	return sf::Vector2f(
		std::round(sprite.getPosition().x + (float)(sprite.getTextureRect().width / 2)),
		std::round(sprite.getPosition().y + (float)(sprite.getTextureRect().height - LevelMap::TileSize() / 2)));
}

void Player::executeAction(Game& game) const
{
	if (action != nullptr)
	{
		game.Events().addBack(action);
	}
}

void Player::MapPosition(Level& level, const MapCoord& pos)
{
	auto oldObj = level.Map()[mapPosition.x][mapPosition.y].object;
	level.Map()[mapPosition.x][mapPosition.y].object = nullptr;
	mapPosition = pos;
	level.Map()[mapPosition.x][mapPosition.y].object = oldObj;
}

void Player::updateWalkPath(Game& game, Level& level, const sf::Vector2u& texSize)
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
		const auto& nextMapPos = walkPath.front();
		if (walkPath.size() == 1)
		{
			const auto levelObj = level.Map()[nextMapPos.x][nextMapPos.y].object;
			if (levelObj != nullptr)
			{
				levelObj->executeAction(game);
				walkPath.pop();
				return;
			}
		}

		setDirection(getPlayerDirection(mapPosition, nextMapPos));
		MapPosition(level, nextMapPos);
		walkPath.pop();
	}

	auto drawPos = level.Map().getCoords(mapPosition);
	drawPos.x += (float)(-((int)texSize.x / 2)) + LevelMap::TileSize();
	drawPos.y += (float)(224 - ((int)texSize.y - LevelMap::TileSize()));
	sprite.setPosition(drawPos);
}

void Player::update(Game& game, Level& level)
{
	auto rect = sprite.getGlobalBounds();
	if (enableHover == true)
	{
		if (level.HasMouseInside() == true &&
			rect.contains(level.MousePositionf()) == true)
		{
			if (hovered == false)
			{
				hovered = true;
				level.setHoverObject(this);
				level.executeHoverEnterAction(game);
			}
		}
		else
		{
			if (hovered == true)
			{
				hovered = false;
				if (level.getHoverObject() == this)
				{
					level.setHoverObject(nullptr);
					level.executeHoverLeaveAction(game);
				}
			}
		}
	}
	if (rect.width > 0 && rect.height > 0)
	{
		updateWalkPath(game, level, sf::Vector2u(rect.width, rect.height));
	}

	if (celTexture == nullptr
		|| frameRange.first > frameRange.second)
	{
		return;
	}

	// add delta time
	currentTime += game.getElapsedTime();

	// if current time is bigger then the frame time advance one frame
	if (currentTime >= frameTime)
	{
		// reset time, but keep the remainder
		currentTime = sf::microseconds(currentTime.asMicroseconds() % frameTime.asMicroseconds());

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
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int(props.first.c_str()))
	{
	case str2int("type"):
		var = Variable("player");
		break;
	case str2int("id"):
		var = Variable(id);
		break;
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
	case str2int("hasItem"):
	{
		std::string props2;
		size_t invIdx;
		size_t itemIdx;
		if (parseInventoryAndItem(props.second, props2, invIdx, itemIdx) == true)
		{
			var = Variable(inventories[invIdx][itemIdx] != nullptr);
			break;
		}
		return false;
	}
	case str2int("isItemSlotInUse"):
	{
		std::string props2;
		size_t invIdx;
		size_t itemIdx;
		if (parseInventoryAndItem(props.second, props2, invIdx, itemIdx) == true)
		{
			var = Variable(inventories[invIdx].isItemSlotInUse(itemIdx));
			break;
		}
		return false;
	}
	case str2int("item"):
	{
		std::string props2;
		size_t invIdx;
		size_t itemIdx;
		if (parseInventoryAndItem(props.second, props2, invIdx, itemIdx) == true)
		{
			auto item = inventories[invIdx][itemIdx].get();
			if (item != nullptr)
			{
				return item->getProperty(props2, var);
			}
		}
		return false;
	}
	case str2int("inventory"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto invIdx = GameUtils::getPlayerInventoryIndex(props2.first);
		if (invIdx < inventories.size())
		{
			return inventories[invIdx].getProperty(props2.second, var);
		}
		return false;
	}
	default:
		return false;
	}
	return true;
}

void Player::setProperty(const std::string& prop, const Variable& val)
{
	if (prop.empty() == true)
	{
		return;
	}
	switch (str2int(prop.c_str()))
	{
	case str2int("lifeBase"):
	{
		if (val.is<int64_t>() == true)
		{
			LifeBase((int32_t)val.get<int64_t>());
		}
	}
	break;
	case str2int("manaBase"):
	{
		if (val.is<int64_t>() == true)
		{
			ManaBase((int32_t)val.get<int64_t>());
		}
	}
	break;
	}
}

bool Player::parseInventoryAndItem(const std::string& str,
	std::string& props, size_t& invIdx, size_t& itemIdx) const
{
	auto strPair = Utils::splitStringIn2(str, '.');
	invIdx = GameUtils::getPlayerInventoryIndex(strPair.first);
	if (invIdx < inventories.size())
	{
		auto strPair2 = Utils::splitStringIn2(strPair.second, '.');
		auto strPair3 = Utils::splitStringIn2(strPair2.first, ',');
		itemIdx = 0;
		if (strPair3.second.empty() == false)
		{
			size_t x = std::strtoul(strPair3.first.c_str(), NULL, 10);
			size_t y = std::strtoul(strPair3.second.c_str(), NULL, 10);
			itemIdx = inventories[invIdx].getIndex(x, y);
		}
		else
		{
			if (invIdx == (size_t)PlayerInventory::Body)
			{
				itemIdx = GameUtils::getPlayerItemMountIndex(strPair2.first);
			}
			else
			{
				itemIdx = std::strtoul(strPair2.first.c_str(), NULL, 10);
			}
		}
		if (itemIdx < inventories[invIdx].Size())
		{
			props = strPair2.second;
			return true;
		}
	}
	return false;
}
