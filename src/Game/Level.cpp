#include "Level.h"
#include "Game.h"
#include "Panel.h"
#include "Player.h"
#include "Utils/Utils.h"

void Level::save(const std::string_view filePath,
	const Save::Properties& props, const Game& game) const
{
	Save::save(filePath, props, game, *this);
}

void Level::serialize(void* serializeObj, const Save::Properties& props,
	const Game& game, const Level& level)
{
	Save::serialize(serializeObj, props, game, *this);
}

bool Level::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("clickedObject"):
	{
		if (auto obj = clickedObject.lock())
		{
			return obj->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("currentPlayer"):
	{
		if (auto obj = currentPlayer.lock())
		{
			return obj->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("hasAutomap"):
		var = Variable(hasAutomap());
		return true;
	case str2int16("hasCurrentPlayer"):
		var = Variable((currentPlayer.expired() == false));
		return true;
	case str2int16("hasQuest"):
		var = Variable(hasQuest(props.second));
		return true;
	case str2int16("hoverObject"):
	{
		if (auto obj = hoverObject.lock())
		{
			return obj->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("id"):
		var = Variable(id);
		return true;
	case str2int16("name"):
		var = Variable(name);
		return true;
	case str2int16("path"):
		var = Variable(path);
		return true;
	case str2int16("levelObject"):
	{
		std::string_view props2;
		auto obj = parseLevelObjectIdOrMapPosition(props.second, props2);
		if (obj != nullptr)
		{
			return obj->getProperty(props2, var);
		}
		break;
	}
	case str2int16("player"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto player = getLevelObject<Player>(props2.first);
		if (player != nullptr)
		{
			return player->getProperty(props2.second, var);
		}
		break;
	}
	case str2int16("quest"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		for (const auto& quest : quests)
		{
			if (quest.Id() == props2.first)
			{
				return quest.getProperty(props2.second, var);
			}
		}
		break;
	}
	case str2int16("spell"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto spell = getSpell(std::string{ props2.first });
		if (spell != nullptr)
		{
			return spell->getProperty(props2.second, var);
		}
		break;
	}
	case str2int16("showAutomap"):
		var = Variable(automapSurface.visible);
		return true;
	case str2int16("zoom"):
		var = Variable((double)zoomValue.getFinal());
		return true;
	case str2int16("zoomPercentage"):
		var = Variable((int64_t)(std::round(zoomValue.getFinal() * 100.f)));
		return true;
	case str2int16("zoomDrawables"):
		var = Variable(zoomDrawables);
		return true;
	default:
		return getUIObjProp(propHash, props.second, var);
	}
	return false;
}

const Queryable* Level::getQueryable(const std::string_view prop) const
{
	if (prop.empty() == true)
	{
		return this;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	const Queryable* queryable = nullptr;
	switch (propHash)
	{
	case str2int16("clickedObject"):
	{
		queryable = clickedObject.lock().get();
		break;
	}
	case str2int16("currentPlayer"):
	{
		queryable = currentPlayer.lock().get();
		break;
	}
	break;
	case str2int16("hoverObject"):
	{
		queryable = hoverObject.lock().get();
		break;
	}
	break;
	case str2int16("player"):
	{
		props = Utils::splitStringIn2(props.second, '.');
		queryable = getLevelObject<Player>(props.first);
	}
	break;
	case str2int16("quest"):
	{
		props = Utils::splitStringIn2(props.second, '.');
		for (const auto& quest : quests)
		{
			if (quest.Id() == props.first)
			{
				queryable = &quest;
				break;
			}
		}
	}
	break;
	default:
		break;
	}
	if (queryable != nullptr &&
		props.second.empty() == false)
	{
		return queryable->getQueryable(props.second);
	}
	return queryable;
}

std::vector<std::variant<const Queryable*, Variable>> Level::getQueryableList(
	const std::string_view prop) const
{
	std::vector<std::variant<const Queryable*, Variable>> queryableList;

	auto props = Utils::splitStringIn2(prop, '.');
	if (props.first.empty() == false)
	{
		if (props.first == "quests")
		{
			for (const auto& quest : quests)
			{
				queryableList.push_back({ &quest });
			}
		}
		else
		{
			auto player = getPlayerOrCurrent(props.first);
			if (player == nullptr)
			{
				player = getCurrentPlayer();
			}
			if (player != nullptr)
			{
				return player->getQueryableList(props.second);
			}
		}
	}
	return queryableList;
}

void Level::draw(const Game& game, sf::RenderTarget& target) const
{
	if (visible == false)
	{
		return;
	}

	auto origView = target.getView();

	// layers

	surface.clear(sf::Color::Black);
	automapSurface.clear(sf::Color::Transparent);

	SpriteShaderCache spriteCache;

	for (size_t i = 0; i < levelLayers.size(); i++)
	{
		const auto& levelLayer = levelLayers[i];

		if (holdsNullLevelLayer(levelLayer.layer) == true)
		{
			continue;
		}
		if (levelLayer.automap == false)
		{
			if (surface.visible == false)
			{
				continue;
			}
		}
		else
		{
			if (automapSurface.visible == false)
			{
				continue;
			}
		}

		auto& layerSurface = (levelLayer.automap == false ? surface : automapSurface);

		layerSurface.updateDrawView(levelLayer.viewportOffset);

		if (holdsTilesetLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<TilesetLevelLayer>(levelLayer.layer);
			layer.draw(layerSurface, spriteCache,
				game.Shaders().Sprite, *this,
				i == indexToDrawLevelObjects,
				levelLayer.automap
			);
		}
		else if (holdsTextureLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<TextureLevelLayer>(levelLayer.layer);
			layer.draw(layerSurface);
		}
		else if (holdsColorLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<ColorLevelLayer>(levelLayer.layer);
			layer.draw(layerSurface);
		}
	}

	// lighting

	if (lights.empty() == false && map.getDefaultLight() < 255)
	{
#if SFML_VERSION_MAJOR >= 2 && SFML_VERSION_MINOR >= 6
		const static sf::BlendMode lightBlend(
			sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add,
			sf::BlendMode::One, sf::BlendMode::Zero, sf::BlendMode::Min
		);
#else
		const static sf::BlendMode lightBlend(
			sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add,
			sf::BlendMode::DstAlpha, sf::BlendMode::Zero, sf::BlendMode::Add
		);
#endif

		sf::RenderStates lightStates;
		lightStates.blendMode = lightBlend;

		for (const auto& light : lights)
		{
			surface.draw(light, lightStates);
		}
	}

	auto surfaceStates(sf::RenderStates::Default);
	if (gameShader != nullptr)
	{
		auto shader = gameShader->shader.get();
		surfaceStates.shader = shader;
		for (auto uniformHash : gameShader->uniforms)
		{
			switch (uniformHash)
			{
			case str2int16("elapsedTime"):
			{
				shader->setUniform("elapsedTime", game.getTotalElapsedTime().asSeconds());
				break;
			}
			case str2int16("mousePosition"):
			{
				if (hasMouseInside == true)
				{
					shader->setUniform("mousePosition", sf::Glsl::Vec2(
						(game.MousePositionf().x - surface.Position().x) /
						surface.Size().x,
						(game.MousePositionf().y - surface.Position().y) /
						surface.Size().y
					));
				}
				break;
			}
			case str2int16("textureSize"):
			{
				shader->setUniform("textureSize", sf::Glsl::Vec2(
					surface.Size().x,
					surface.Size().y
				));
				break;
			}
			case str2int16("visibleRect"):
			{
				shader->setUniform("visibleRect", sf::Glsl::Vec4(
					surface.visibleRect.left,
					surface.visibleRect.top,
					surface.visibleRect.width,
					surface.visibleRect.height
				));
				break;
			}
			case str2int16("defaultLight"):
			{
				shader->setUniform("defaultLight", (float)(255 - map.getDefaultLight()) / 255);
				break;
			}
			default:
				break;
			}
		}
	}
	surface.draw(target, surfaceStates);

	// level drawables

	if (drawables.empty() == false)
	{
		if (zoomDrawables == true)
		{
			surface.clear(sf::Color::Transparent);
			bool hasDrawn = false;
			for (const auto& item : drawables)
			{
				if (auto obj = item.drawable.lock())
				{
					hasDrawn |= surface.draw(game, *obj);
				}
			}
			if (hasDrawn == true)
			{
				surface.draw(target);
			}
		}
		else
		{
			for (const auto& item : drawables)
			{
				if (auto obj = item.drawable.lock())
				{
					obj->draw(game, target);
				}
			}
		}
	}

	// automap

	automapSurface.draw(target, sf::RenderStates::Default);

	target.setView(origView);
}

void Level::processInputEvents(Game& game)
{
	if (game.Resources().hasActiveInputEvents() == false)
	{
		return;
	}
	if (auto player = currentPlayer.lock())
	{
		if (player->blockWalk() == true)
		{
			return;
		}
		PlayerDirection direction(PlayerDirection::All);
		if (game.Resources().hasActiveInputEvents({ moveUpEventHash, moveLeftEventHash }) == true)
		{
			direction = PlayerDirection::BackLeft;
		}
		else if (game.Resources().hasActiveInputEvents({ moveUpEventHash, moveRightEventHash }) == true)
		{
			direction = PlayerDirection::BackRight;
		}
		else if (game.Resources().hasActiveInputEvents({ moveDownEventHash, moveLeftEventHash }) == true)
		{
			direction = PlayerDirection::FrontLeft;
		}
		else if (game.Resources().hasActiveInputEvents({ moveDownEventHash, moveRightEventHash }) == true)
		{
			direction = PlayerDirection::FrontRight;
		}
		else if (game.Resources().hasActiveInputEvents({ moveUpEventHash }) == true)
		{
			direction = PlayerDirection::Back;
		}
		else if (game.Resources().hasActiveInputEvents({ moveDownEventHash }) == true)
		{
			direction = PlayerDirection::Front;
		}
		else if (game.Resources().hasActiveInputEvents({ moveLeftEventHash }) == true)
		{
			direction = PlayerDirection::Left;
		}
		else if (game.Resources().hasActiveInputEvents({ moveRightEventHash }) == true)
		{
			direction = PlayerDirection::Right;
		}
		bool doAction = game.Resources().hasActiveInputEvents({ doActionEventHash });
		player->Walk(map, direction, doAction);
	}
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
	processInputEvents(game);

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

	sf::FloatRect rect(surface.Position(), surface.Size());
	if (rect.contains(game.MousePositionf()) == true)
	{
		hasMouseInside = true;

		if (game.wasMousePressed() == true)
		{
			onMouseButtonPressed(game);
		}
		if (game.wasMouseScrolled() == true)
		{
			onMouseScrolled(game);
		}
		if (game.hasTouchBegan() == true)
		{
			onTouchBegan(game);
		}
		if (captureInputEvents != InputEventType::None)
		{
			game.clearInputEvents(captureInputEvents);
		}
	}
	else
	{
		hasMouseInside = false;
	}
	for (auto& obj : levelObjects)
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

	updateDrawables(game);
}

void Level::clearPlayerClasses()
{
	for (auto it = levelObjectClasses.begin(); it != levelObjectClasses.end();)
	{
		auto plrClass = dynamic_cast<PlayerClass*>(it->second.get());
		if (plrClass != nullptr)
		{
			bool classBeingUsed = false;
			for (const auto& obj : levelObjects)
			{
				if (obj->getBaseClass() == it->second.get())
				{
					classBeingUsed = true;
					break;
				}
			}
			if (classBeingUsed == false)
			{
				it = levelObjectClasses.erase(it);
				continue;
			}
		}
		++it;
	}
}

void Level::clearPlayerTextures() noexcept
{
	for (const auto& classObj : levelObjectClasses)
	{
		auto plrClass = dynamic_cast<PlayerClass*>(classObj.second.get());
		if (plrClass == nullptr)
		{
			continue;
		}
		bool classIsUsed = false;
		for (const auto& obj : levelObjects)
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

Player* Level::getPlayerOrCurrent(const std::string_view id) const noexcept
{
	if (id.empty() == true)
	{
		return currentPlayer.lock().get();
	}
	return getLevelObject<Player>(id);
}

void Level::setCurrentPlayer(std::weak_ptr<Player> player_) noexcept
{
	currentPlayer = player_;
	updateCurrentMapViewCenter(false);
}

bool Level::updateCurrentMapViewCenter(bool smooth) noexcept
{
	if (followCurrentPlayer == true)
	{
		if (auto obj = currentPlayer.lock())
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
	if (map.isMapCoordValid(mapCoord) == true)
	{
		return map[mapCoord].getObject<Item>();
	}
	return nullptr;
}

Item* Level::getItem(const ItemCoordInventory& itemCoord) const
{
	Player* player;
	return getItem(itemCoord, player);
}

Item* Level::getItem(const ItemCoordInventory& itemCoord, Player*& player) const
{
	player = getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			return player->SelectedItem();
		}
		auto& inventory = player->getInventory(itemCoord.getInventoryIdx());
		if (itemCoord.isCoordXY() == true)
		{
			return inventory.get(itemCoord.getItemXY());
		}
		else
		{
			return inventory.get(itemCoord.getItemIdx());
		}
	}
	return nullptr;
}

Item* Level::getItem(const ItemLocation& location) const
{
	Player* player;
	return getItem(location, player);
}

Item* Level::getItem(const ItemLocation& location, Player*& player) const
{
	if (holdsMapCoord(location) == true)
	{
		player = nullptr;
		return getItem(std::get<PairFloat>(location));
	}
	else
	{
		return getItem(std::get<ItemCoordInventory>(location), player);
	}
}

std::shared_ptr<Item> Level::removeItem(const PairFloat& mapCoord)
{
	auto item = map.removeLevelObject<Item>(mapCoord);
	if (item != nullptr)
	{
		item->clearMapPosition();
		return removeLevelObject<Item>(item);
	}
	return nullptr;
}

std::shared_ptr<Item> Level::removeItem(const ItemCoordInventory& itemCoord)
{
	Player* player;
	return removeItem(itemCoord, player);
}

std::shared_ptr<Item> Level::removeItem(const ItemCoordInventory& itemCoord, Player*& player)
{
	player = getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			return player->SelectedItem(nullptr);
		}
		std::shared_ptr<Item> nullItem;
		auto& inventory = player->getInventory(itemCoord.getInventoryIdx());
		if (itemCoord.isCoordXY() == true)
		{
			std::shared_ptr<Item> oldItem;
			inventory.set(itemCoord.getItemXY(), nullItem, oldItem);
			return oldItem;
		}
		else
		{
			std::shared_ptr<Item> oldItem;
			inventory.set(itemCoord.getItemIdx(), nullItem, oldItem);
			return oldItem;
		}
	}
	return nullptr;
}

std::shared_ptr<Item> Level::removeItem(const ItemLocation& location)
{
	Player* player;
	return removeItem(location, player);
}

std::shared_ptr<Item> Level::removeItem(const ItemLocation& location, Player*& player)
{
	if (holdsMapCoord(location) == true)
	{
		player = nullptr;
		return removeItem(std::get<PairFloat>(location));
	}
	else
	{
		return removeItem(std::get<ItemCoordInventory>(location), player);
	}
}

bool Level::setItem(const PairFloat& mapCoord, std::shared_ptr<Item>& item)
{
	if (map.isMapCoordValid(mapCoord) == false)
	{
		return false;
	}
	auto& mapCell = map[mapCoord];
	auto oldItem = mapCell.getObject<Item>();
	if (item == nullptr)
	{
		if (oldItem != nullptr)
		{
			oldItem->remove(map);
			removeLevelObject<Item>(oldItem);
		}
		return true;
	}
	if (mapCell.PassableIgnoreObject(currentPlayer.lock().get()) == true
		&& oldItem == nullptr)
	{
		addLevelObject(std::move(item), mapCoord);
		return true;
	}
	return false;
}

bool Level::setItem(const ItemCoordInventory& itemCoord, std::shared_ptr<Item>& item)
{
	auto player = getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			player->SelectedItem(std::move(item));
			return true;
		}
		auto invIdx = itemCoord.getInventoryIdx();
		if (invIdx < player->getInventorySize())
		{
			size_t itemIdx;
			if (itemCoord.isCoordXY() == true)
			{
				itemIdx = player->getInventory(invIdx)
					.getIndex(itemCoord.getItemXY());
			}
			else
			{
				itemIdx = itemCoord.getItemIdx();
			}
			return player->setItem(invIdx, itemIdx, item);
		}
	}
	return false;
}

bool Level::setItem(const ItemLocation& location, std::shared_ptr<Item>& item)
{
	if (holdsMapCoord(location) == true)
	{
		return setItem(std::get<PairFloat>(location), item);
	}
	else
	{
		return setItem(std::get<ItemCoordInventory>(location), item);
	}
}

bool Level::deleteItem(const PairFloat& mapCoord)
{
	std::shared_ptr<Item> nullItem;
	return setItem(mapCoord, nullItem);
}

bool Level::deleteItem(const ItemCoordInventory& itemCoord)
{
	std::shared_ptr<Item> nullItem;
	return setItem(itemCoord, nullItem);
}

bool Level::deleteItem(const ItemLocation& location)
{
	std::shared_ptr<Item> nullItem;
	return setItem(location, nullItem);
}

LevelObjValue Level::addItemQuantity(const ItemLocation& location, LevelObjValue amount)
{
	Player* player;
	auto item = getItem(location, player);
	if (item != nullptr)
	{
		if (amount != 0)
		{
			LevelObjValue newAmount = amount;
			auto newQuant = item->addQuantity(newAmount);

			if (player != nullptr &&
				std::get<ItemCoordInventory>(location).isSelectedItem() == false)
			{
				player->updateItemQuantityCache(
					item->Class()->IdHash16(), (amount - newAmount));
			}
			if (newQuant == 0)
			{
				removeItem(location);
			}
		}
	}
	return amount;
}

void Level::addQuest(Quest quest_)
{
	for (const auto& quest : quests)
	{
		if (quest.Id() == quest_.Id())
		{
			return;
		}
	}
	quests.push_back(std::move(quest_));
}

void Level::clearQuests()
{
	quests.clear();
}

void Level::deleteQuest(const std::string_view questId)
{
	for (auto it = quests.begin(); it != quests.end(); ++it)
	{
		if (it->Id() == questId)
		{
			quests.erase(it);
			return;
		}
	}
}

bool Level::hasQuest(const std::string_view questId) const noexcept
{
	for (auto& quest : quests)
	{
		if (quest.Id() == questId)
		{
			return true;
		}
	}
	return false;
}

void Level::setQuestState(const std::string_view questId, int state) noexcept
{
	for (auto& quest : quests)
	{
		if (quest.Id() == questId)
		{
			quest.State(state);
			return;
		}
	}
}

bool Level::hasSpell(const std::string_view id) const
{
	return getClass<Spell>(id) != nullptr;
}

Spell* Level::getSpell(const std::string_view id) const
{
	return getClass<Spell>(id);
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
