#include "Level.h"
#include "Game/Game.h"
#include "Game/Player/Player.h"
#include "LevelDraw.h"
#include "LevelItem.h"
#include "LevelQuest.h"
#include "LevelSave.h"
#include "LevelUIObject.h"

void Level::save(const std::string_view filePath, const UnorderedStringMap<Variable>& props) const
{
	LevelSave::save(*this, filePath, props);
}

std::shared_ptr<Action> Level::getAction(uint16_t nameHash16) const noexcept
{
	return LevelUIObject::getAction(*this, nameHash16);
}

bool Level::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	return LevelUIObject::setAction(*this, nameHash16, action);
}

bool Level::getProperty(const std::string_view prop, Variable& var) const
{
	return LevelUIObject::getProperty(*this, prop, var);
}

QueryObject Level::getQueryable(const std::string_view prop) const
{
	return LevelUIObject::getQueryable(*this, prop);
}

std::vector<VarOrQueryObject> Level::getQueryableList(const std::string_view prop) const
{
	return LevelUIObject::getQueryableList(*this, prop);
}

void Level::draw(const Game& game, sf::RenderTarget& target) const
{
	LevelDraw::draw(*this, game, target);
}

void Level::update(Game& game)
{
	if (visible == false)
	{
		return;
	}
	if (viewNeedsUpdate == true)
	{
		surface.updateView(game);
		automapSurface.updateView(game);
		updateTilesetLayersVisibleArea();
	}
	if (pause == true)
	{
		return;
	}

	updateZoom(game);
	updateMouse(game);
	updateLights();

	inputManager.processInput(*this, game);

	epoch++;

	for (auto& layer : levelLayers)
	{
		if (holdsTilesetLevelLayer(layer.layer) == true)
		{
			auto tiles = std::get<TilesetLevelLayer>(layer.layer).tiles.get();
			if (tiles != nullptr)
			{
				tiles->update(epoch, game.getElapsedTime());
			}
		}
		if (holdsTextureLevelLayer(layer.layer) == true)
		{
			std::get<TextureLevelLayer>(layer.layer).update(game.getElapsedTime());
		}
	}

	for (auto& obj : levelObjects.Objects())
	{
		obj->update(game, *this, obj);
	}
	if (currentMapPosition.x == -1.f &&
		currentMapPosition.y == -1.f)
	{
		if (updateCurrentMapViewCenter(false) == false)
		{
			setCurrentMapPosition(
				{ map.MapSizef().x / 2.f, map.MapSizef().y / 2.f },
				false
			);
		}
	}
	if (smoothMovement == true)
	{
		auto elapsedSeconds = game.getElapsedTime().asSeconds();
		currentMapViewCenterX.update(elapsedSeconds);
		currentMapViewCenterY.update(elapsedSeconds);
	}
	updateCurrentMapViewCenter(smoothMovement);
	auto newViewCenterX = std::round(currentMapViewCenterX.get());
	auto newViewCenterY = std::round(currentMapViewCenterY.get());
	if (surface.getCenter().x != newViewCenterX ||
		surface.getCenter().y != newViewCenterY ||
		viewNeedsUpdate == true)
	{
		surface.setCenter(newViewCenterX, newViewCenterY);
		updateCurrentAutomapViewCenter();
		automapSurface.setCenter(currentAutomapViewCenter);
		updateTilesetLayersVisibleArea();
	}
	if (viewNeedsUpdate == true)
	{
		viewNeedsUpdate = false;
	}

	drawables.update(*this, game);
}

void Level::clearPlayerClasses()
{
	levelObjects.clearClasses<PlayerClass>();
}

void Level::clearPlayerTextures() noexcept
{
	for (const auto& classObj : levelObjects.Classes())
	{
		auto plrClass = dynamic_cast<PlayerClass*>(classObj.second.get());
		if (plrClass == nullptr)
		{
			continue;
		}
		bool classIsUsed = false;
		for (const auto& obj : levelObjects.Objects())
		{
			if (plrClass == obj->getBaseClass())
			{
				classIsUsed = true;
				break;
			}
		}
		if (classIsUsed == false)
		{
			plrClass->clearTextures();
		}
	}
}

std::shared_ptr<Player> Level::getPlayerOrCurrent(const std::string_view id) const noexcept
{
	if (id.empty() == true)
	{
		return levelObjects.CurrentPlayer();
	}
	return levelObjects.getSharedPtr<Player>(id);
}

void Level::setCurrentPlayer(std::weak_ptr<Player> player_) noexcept
{
	levelObjects.currentPlayer = player_;
	updateCurrentMapViewCenter(false);
}

bool Level::updateCurrentMapViewCenter(bool smooth) noexcept
{
	if (followCurrentPlayer == true)
	{
		if (auto obj = levelObjects.CurrentPlayer())
		{
			currentMapPosition = obj->MapPosition();
			setCurrentMapViewCenter(obj->getBasePosition(), smooth);
			return true;
		}
	}
	return false;
}

Item* Level::getItem(const PairFloat& mapCoord) const noexcept
{
	return LevelItem::getItem(*this, mapCoord);
}

Item* Level::getItem(const ItemCoordInventory& itemCoord) const
{
	return LevelItem::getItem(*this, itemCoord);
}

Item* Level::getItem(const ItemCoordInventory& itemCoord, std::shared_ptr<Player>& player) const
{
	return LevelItem::getItem(*this, itemCoord, player);
}

Item* Level::getItem(const ItemLocation& location) const
{
	return LevelItem::getItem(*this, location);
}

Item* Level::getItem(const ItemLocation& location, std::shared_ptr<Player>& player) const
{
	return LevelItem::getItem(*this, location, player);
}

std::shared_ptr<Item> Level::removeItem(const PairFloat& mapCoord)
{
	return LevelItem::removeItem(*this, mapCoord);
}

std::shared_ptr<Item> Level::removeItem(const ItemCoordInventory& itemCoord)
{
	return LevelItem::removeItem(*this, itemCoord);
}

std::shared_ptr<Item> Level::removeItem(const ItemCoordInventory& itemCoord, std::shared_ptr<Player>& player)
{
	return LevelItem::removeItem(*this, itemCoord, player);
}

std::shared_ptr<Item> Level::removeItem(const ItemLocation& location)
{
	return LevelItem::removeItem(*this, location);
}

std::shared_ptr<Item> Level::removeItem(const ItemLocation& location, std::shared_ptr<Player>& player)
{
	return LevelItem::removeItem(*this, location, player);
}

bool Level::setItem(const PairFloat& mapCoord, std::shared_ptr<Item>& item)
{
	return LevelItem::setItem(*this, mapCoord, item);
}

bool Level::setItem(const ItemCoordInventory& itemCoord, std::shared_ptr<Item>& item)
{
	return LevelItem::setItem(*this, itemCoord, item);
}

bool Level::setItem(const ItemLocation& location, std::shared_ptr<Item>& item)
{
	return LevelItem::setItem(*this, location, item);
}

bool Level::deleteItem(const PairFloat& mapCoord)
{
	return LevelItem::deleteItem(*this, mapCoord);
}

bool Level::deleteItem(const ItemCoordInventory& itemCoord)
{
	return LevelItem::deleteItem(*this, itemCoord);
}

bool Level::deleteItem(const ItemLocation& location)
{
	return LevelItem::deleteItem(*this, location);
}

LevelObjValue Level::addItemQuantity(const ItemLocation& location, LevelObjValue amount)
{
	return LevelItem::addItemQuantity(*this, location, amount);
}

void Level::addQuest(Quest quest_)
{
	LevelQuest::addQuest(*this, quest_);
}

void Level::clearQuests()
{
	quests.clear();
}

void Level::deleteQuest(const std::string_view questId)
{
	LevelQuest::deleteQuest(*this, questId);
}

bool Level::hasQuest(const std::string_view questId) const noexcept
{
	return LevelQuest::hasQuest(*this, questId);
}

void Level::setQuestState(const std::string_view questId, int state) noexcept
{
	LevelQuest::setQuestState(*this, questId, state);
}

bool Level::hasSpell(const std::string_view id) const
{
	return levelObjects.getClass<Spell>(id) != nullptr;
}

Spell* Level::getSpell(const std::string_view id) const
{
	return levelObjects.getClass<Spell>(id);
}

uint32_t Level::getExperienceFromLevel(uint32_t level) const
{
	if (level < experiencePoints.size())
	{
		return experiencePoints[level];
	}
	else if (experiencePoints.empty() == false)
	{
		return experiencePoints.back();
	}
	return 0;
}

uint32_t Level::getLevelFromExperience(uint32_t experience) const
{
	if (experience == 0 || experiencePoints.size() <= 1)
	{
		return 1;
	}
	for (uint32_t i = 1; i < experiencePoints.size(); i++)
	{
		auto exp = experiencePoints[i];
		if (experience <= exp)
		{
			return i;
		}
	}
	return (uint32_t)experiencePoints.size();
}
