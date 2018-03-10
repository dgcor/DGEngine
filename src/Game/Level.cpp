#include "Level.h"
#include "Game.h"
#include "GameUtils.h"
#include "Shaders.h"
#include "Utils/Utils.h"

void Level::Init(LevelMap map_,
	std::shared_ptr<TexturePack> tilesBottom_,
	std::shared_ptr<TexturePack> tilesTop_,
	int tileWidth_, int tileHeight_)
{
	map = std::move(map_);
	tilesBottom = tilesBottom_;
	tilesTop = tilesTop_;
	clickedObject = nullptr;
	hoverObject = nullptr;

	tileWidth = tileWidth_;
	tileHeight = tileHeight_;
	map.setTileSize(tileWidth, tileHeight);

	setCurrentMapPosition(MapCoord(map.Width() / 2, map.Height() / 2));

	updateLevelObjectPositions();
	updateViewPort = true;
}

void Level::updateLevelObjectPositions()
{
	for (const auto& obj : levelObjects)
	{
		const auto& mapPosition = obj->MapPosition();
		if (map.isMapCoordValid(mapPosition) == true)
		{
			map[mapPosition].addBack(obj.get());
		}
	}
	for (const auto& obj : players)
	{
		const auto& mapPosition = obj->MapPosition();
		if (map.isMapCoordValid(mapPosition) == true)
		{
			map[mapPosition].addBack(obj.get());
		}
	}
}

std::shared_ptr<Action> Level::getAction(uint16_t nameHash16) const noexcept
{
	switch (nameHash16)
	{
	case str2int16("click"):
	case str2int16("leftClick"):
		return leftAction;
	case str2int16("rightClick"):
		return rightAction;
	case str2int16("hoverEnter"):
		return hoverEnterAction;
	case str2int16("hoverLeave"):
		return hoverLeaveAction;
	case str2int16("scrollDown"):
		return scrollDownAction;
	case str2int16("scrollUp"):
		return scrollUpAction;
	default:
		return nullptr;
	}
}

bool Level::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("click"):
	case str2int16("leftClick"):
		leftAction = action;
		break;
	case str2int16("rightClick"):
		rightAction = action;
		break;
	case str2int16("hoverEnter"):
		hoverEnterAction = action;
		break;
	case str2int16("hoverLeave"):
		hoverLeaveAction = action;
		break;
	case str2int16("scrollDown"):
		scrollDownAction = action;
		break;
	case str2int16("scrollUp"):
		scrollUpAction = action;
		break;
	default:
		return false;
	}
	return true;
}

void Level::executeHoverEnterAction(Game& game)
{
	if (hoverEnterAction != nullptr)
	{
		game.Events().addBack(hoverEnterAction);
	}
}

void Level::executeHoverLeaveAction(Game& game)
{
	if (hoverLeaveAction != nullptr)
	{
		game.Events().addFront(hoverLeaveAction);
	}
}

void Level::Zoom(float factor, bool smooth) noexcept
{
	if (factor > 2.f)
	{
		factor = 2.f;
	}
	else if (factor < 0.5f)
	{
		factor = 0.5f;
	}
	smoothZoom = smooth;
	if (smooth == false)
	{
		currentZoomFactor = factor;
	}
	else
	{
		zoomStepStart = 0.f;
		startZoomFactor = currentZoomFactor;
		diffZoomFactor = factor - startZoomFactor;
	}
	stopZoomFactor = factor;
	hasNewZoom = true;
}

// Quart ease out function
// http://easings.net/
// t is the current time.
// b is the beginning value of the property.
// c is the change between the beginning and destination value of the property.
// d is the final time.
static float easeOutFunction(float t, float b, float c, float d) noexcept
{
	t = t / d - 1;
	return -c * (t*t*t*t - 1) + b;
}

void::Level::updateZoom(const Game& game)
{
	if (hasNewZoom == false)
	{
		return;
	}
	if (smoothZoom == false)
	{
		hasNewZoom = false;
	}
	else
	{
		zoomStepStart += game.getElapsedTime().asSeconds();

		if (zoomStepStart >= zoomStepStop)
		{
			zoomStepStart = zoomStepStop;
			hasNewZoom = false;
		}

		currentZoomFactor = easeOutFunction(
			zoomStepStart,
			startZoomFactor,
			diffZoomFactor,
			zoomStepStop);
	}

	view.setZoom(1.f / currentZoomFactor);

	view.updateViewport(game);
	updateVisibleArea();
}

void Level::addLevelObject(std::unique_ptr<LevelObject> obj, bool addToFront)
{
	auto mapPos = obj->MapPosition();
	addLevelObject(std::move(obj), mapPos, addToFront);
}

void Level::addLevelObject(std::unique_ptr<LevelObject> obj,
	const MapCoord& mapCoord, bool addToFront)
{
	addLevelObject(std::move(obj), map[mapCoord], addToFront);
}

void Level::addLevelObject(std::unique_ptr<LevelObject> obj,
	LevelCell& mapCell, bool addToFront)
{
	if (addToFront == true)
	{
		mapCell.addFront(obj.get());
	}
	else
	{
		mapCell.addBack(obj.get());
	}
	levelObjects.push_back(std::move(obj));
}

void Level::addPlayer(std::unique_ptr<Player> player)
{
	auto mapPos = player->MapPosition();
	addPlayer(std::move(player), mapPos);
}

void Level::addPlayer(std::unique_ptr<Player> player, const MapCoord& mapCoord)
{
	player->MapPosition(*this, mapCoord);
	player->updateDrawPosition(*this);
	players.push_back(std::move(player));
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == false)
	{
		return;
	}

	auto origView = target.getView();
	target.setView(view.getView());

	sf::Sprite sprite;
	TextureInfo ti;
	sf::FloatRect tileRect;
	const sf::Texture* palette = nullptr;

	auto tiles = tilesBottom.get();
	if (tiles != nullptr)
	{
		if (tiles->isIndexed() == true &&
			tiles->getPalette() != nullptr &&
			Shaders::supportsPalettes() == true)
		{
			palette = &tiles->getPalette()->texture;
		}
		if (palette != nullptr)
		{
			states.shader = &Shaders::Palette;
			Shaders::Palette.setUniform("palette", *palette);
		}

		for (Coord x = visibleStart.x; x < visibleEnd.x; x++)
		{
			for (Coord y = visibleStart.y; y < visibleEnd.y; y++)
			{
				auto index = map[x][y].TileIndexBack();
				if (index < 0)
				{
					continue;
				}
				if (tiles->get((size_t)index, ti) == true)
				{
					auto coords = map.getCoord(MapCoord(x, y));
					coords.y -= (float)(ti.textureRect.height - tileHeight);
					coords += ti.offset;
					tileRect.left = coords.x;
					tileRect.top = coords.y;
					tileRect.width = (float)ti.textureRect.width;
					tileRect.height = (float)ti.textureRect.height;
					if (visibleRect.intersects(tileRect) == true)
					{
						sprite.setTexture(*ti.texture);
						sprite.setTextureRect(ti.textureRect);
						sprite.setPosition(coords);
						target.draw(sprite, states);
					}
				}
			}
		}
	}

	palette = nullptr;
	tiles = tilesTop.get();

	if (tiles != nullptr &&
		tiles->isIndexed() == true &&
		tiles->getPalette() != nullptr &&
		Shaders::supportsPalettes() == true)
	{
		palette = &tiles->getPalette()->texture;
	}
	if (palette != nullptr)
	{
		states.shader = &Shaders::Palette;
		Shaders::Palette.setUniform("palette", *palette);
	}
	else
	{
		states.shader = NULL;
	}

	for (Coord x = visibleStart.x; x < visibleEnd.x; x++)
	{
		for (Coord y = visibleStart.y; y < visibleEnd.y; y++)
		{
			auto coords = map.getCoord(MapCoord(x, y));
			bool drewObj = false;

			for (const auto& drawObj : map[x][y])
			{
				if (drawObj != nullptr)
				{
					target.draw(*drawObj, states);
					drewObj = true;
				}
			}
			if (drewObj == true &&
				palette != nullptr)
			{
				Shaders::Palette.setUniform("palette", *palette);
			}
			if (tiles != nullptr)
			{
				size_t index = map[x][y].TileIndexFront();
				if (index < 0)
				{
					continue;
				}
				if (tiles->get((size_t)index, ti) == true)
				{
					coords.y -= (float)(ti.textureRect.height - tileHeight);
					coords += ti.offset;
					tileRect.left = coords.x;
					tileRect.top = coords.y;
					tileRect.width = (float)ti.textureRect.width;
					tileRect.height = (float)ti.textureRect.height;
					if (visibleRect.intersects(tileRect) == true)
					{
						sprite.setTexture(*ti.texture);
						sprite.setTextureRect(ti.textureRect);
						sprite.setPosition(coords);
						target.draw(sprite, states);
					}
				}
			}
		}
	}
	target.setView(origView);
}

void Level::updateSize(const Game& game)
{
	view.updateSize(game);
	updateVisibleArea();
}

void Level::updateVisibleArea()
{
	const auto& viewCenter = view.getCenter();
	const auto& viewSize = view.getVisibleSize();

	visibleRect = sf::FloatRect(viewCenter.x - std::round(viewSize.x / 2),
		viewCenter.y - std::round(viewSize.y / 2),
		std::round(viewSize.x),
		std::round(viewSize.y));

	sf::Vector2f TL{ visibleRect.left, visibleRect.top };
	sf::Vector2f TR{ TL.x + visibleRect.width + tileWidth, TL.y };
	sf::Vector2f BL{ TL.x, TL.y + visibleRect.height + tileHeight };
	sf::Vector2f BR{ TR.x, BL.y };

	auto mapTL = map.getTile(TL);
	auto mapTR = map.getTile(TR);
	auto mapBL = map.getTile(BL);
	auto mapBR = map.getTile(BR);

	if (mapTL.x > 0)
	{
		visibleStart.x = std::max((Coord)0, mapTL.x - 2);
	}
	else
	{
		visibleStart.x = 0;
	}
	visibleEnd.x = std::clamp(mapBR.x + 12, 0, map.Width());
	if (mapTR.y > 0)
	{
		visibleStart.y = std::max((Coord)0, mapTR.y - 2);
	}
	else
	{
		visibleStart.y = 0;
	}
	visibleEnd.y = std::clamp(mapBL.y + 12, 0, map.Height());
}

void Level::updateMouse(const Game& game)
{
	mousePositionf = view.getPosition(game.MousePositionf());
	auto mousePositionf2 = mousePositionf;
	mousePositionf2.y += 224;
	mapCoordOverMouse = map.getTile(mousePositionf2);
}

void Level::onMouseButtonPressed(Game& game)
{
	game.clearMousePressed();
	switch (game.MousePress().button)
	{
	case sf::Mouse::Left:
	{
		clickedMapPosition = getMapCoordOverMouse();
		clickedObject = nullptr;
		if (leftAction != nullptr)
		{
			game.Events().addBack(leftAction);
		}
	}
	break;
	case sf::Mouse::Right:
	{
		if (rightAction != nullptr)
		{
			game.Events().addBack(rightAction);
		}
	}
	break;
	default:
		break;
	}
}

void Level::onMouseScrolled(Game& game)
{
	game.clearMouseScrolled();
	if (game.MouseScroll().delta < 0.f)
	{
		if (scrollDownAction != nullptr)
		{
			game.Events().addBack(scrollDownAction);
		}
	}
	else
	{
		if (scrollUpAction != nullptr)
		{
			game.Events().addBack(scrollUpAction);
		}
	}
}

void Level::onTouchBegan(Game& game)
{
	game.clearTouchBegan();
	switch (game.TouchBegan().finger)
	{
	case 0:
	{
		clickedMapPosition = getMapCoordOverMouse();
		clickedObject = nullptr;
		if (leftAction != nullptr)
		{
			game.Events().addBack(leftAction);
		}
	}
	break;
	case 1:
	{
		if (rightAction != nullptr)
		{
			game.Events().addBack(rightAction);
		}
	}
	break;
	default:
		break;
	}
}

void Level::update(Game& game)
{
	if (visible == false)
	{
		return;
	}
	if (updateViewPort == true)
	{
		updateViewPort = false;
		view.updateViewport(game);
		updateVisibleArea();
	}
	if (pause == true)
	{
		return;
	}

	updateZoom(game);
	updateMouse(game);

	sf::FloatRect rect(view.getPosition(), view.getSize());
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
		if (captureInputEvents == true)
		{
			game.clearInputEvents();
		}
	}
	else
	{
		hasMouseInside = false;
	}
	for (auto& obj : levelObjects)
	{
		obj->update(game, *this);
	}
	for (auto& player : players)
	{
		player->update(game, *this);
	}
	if (followCurrentPlayer == true && currentPlayer != nullptr)
	{
		currentMapPosition = currentPlayer->MapPosition();
		currentMapViewCenter = currentPlayer->getBasePosition();
	}
	if (view.getCenter() != currentMapViewCenter)
	{
		view.setCenter(currentMapViewCenter);
		updateVisibleArea();
	}
}

bool Level::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first.c_str());
	switch (propHash)
	{
	case str2int16("clickedObject"):
	{
		if (clickedObject != nullptr)
		{
			return clickedObject->getProperty(props.second, var);
		}
	}
	break;
	case str2int16("currentPlayer"):
	{
		if (currentPlayer != nullptr)
		{
			return currentPlayer->getProperty(props.second, var);
		}
	}
	break;
	case str2int16("hasCurrentPlayer"):
		var = Variable((bool)(currentPlayer != nullptr));
		return true;
		break;
	case str2int16("hoverObject"):
	{
		if (hoverObject != nullptr)
		{
			return hoverObject->getProperty(props.second, var);
		}
	}
	break;
	case str2int16("name"):
		var = Variable(name);
		return true;
		break;
	case str2int16("player"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		for (const auto& player : players)
		{
			if (player->Id() == props2.first)
			{
				return player->getProperty(props2.second, var);
			}
		}
	}
	break;
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
	}
	case str2int16("zoom"):
		var = Variable((double)stopZoomFactor);
		return true;
		break;
	case str2int16("zoomPercentage"):
		var = Variable((int64_t)(std::roundf(stopZoomFactor * 100.f)));
		return true;
		break;
	default:
		return GameUtils::getUIObjProp(*this, propHash, props.second, var);
	}
	return false;
}

const Queryable* Level::getQueryable(const std::string& prop) const
{
	if (prop.empty() == true)
	{
		return this;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first.c_str());
	const Queryable* queryable = nullptr;
	switch (propHash)
	{
	case str2int16("clickedObject"):
	{
		queryable = clickedObject;
		break;
	}
	case str2int16("currentPlayer"):
	{
		queryable = currentPlayer;
		break;
	}
	break;
	case str2int16("hoverObject"):
	{
		queryable = hoverObject;
		break;
	}
	break;
	case str2int16("player"):
	{
		props = Utils::splitStringIn2(props.second, '.');
		for (const auto& player : players)
		{
			if (player->Id() == props.first)
			{
				queryable = player.get();
				break;
			}
		}
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

void Level::clearLevelObjects()
{
	for (const auto& obj : levelObjects)
	{
		const auto& mapPos = obj->MapPosition();
		if (map.isMapCoordValid(mapPos) == true)
		{
			map[mapPos].removeObject(obj.get());
		}
	}
	levelObjects.clear();
}

Player* Level::getPlayer(const std::string& id) const noexcept
{
	for (const auto& player : players)
	{
		if (player->Id() == id)
		{
			return player.get();
		}
	}
	return nullptr;
}

Player* Level::getPlayerOrCurrent(const std::string& id) const noexcept
{
	if (id.empty() == true)
	{
		return currentPlayer;
	}
	return getPlayer(id);
}

void Level::clearPlayerClasses(size_t clearIdx)
{
	if (clearIdx < playerClasses.size())
	{
		auto removeIfFunc = [&](const auto& cls) {
			auto it = std::find_if(players.begin(), players.end(), [&](const auto& plr) {
				return plr->getPlayerClass() == cls.second.get();
			});
			return it == players.end();
		};

		playerClasses.erase(
			std::remove_if(playerClasses.begin() + clearIdx,
				playerClasses.end(),
				removeIfFunc),
			playerClasses.end());
	}
}

void Level::clearPlayerTextures() noexcept
{
	for (const auto& playerClass : playerClasses)
	{
		auto plrClassPtr = playerClass.second.get();
		bool classIsUsed = false;
		for (const auto& player : players)
		{
			if (plrClassPtr == player->getPlayerClass())
			{
				classIsUsed = true;
				break;
			}
		}
		if (classIsUsed == false)
		{
			plrClassPtr->clearTextures();
		}
	}
}

void Level::clearPlayers(size_t clearIdx)
{
	if (clearIdx < players.size())
	{
		for (size_t i = clearIdx; i < players.size(); i++)
		{
			const auto& mapPos = players[i]->MapPosition();
			if (map.isMapCoordValid(mapPos) == true)
			{
				map[mapPos].removeObject(players[i].get());
			}
		}
		players.erase(players.begin() + clearIdx, players.end());
	}
	for (const auto& player : players)
	{
		if (player.get() == currentPlayer)
		{
			return;
		}
	}
	currentPlayer = nullptr;
}

Item* Level::getItem(const MapCoord& mapCoord) const noexcept
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
	if (std::holds_alternative<MapCoord>(location) == true)
	{
		player = nullptr;
		return getItem(std::get<MapCoord>(location));
	}
	else
	{
		return getItem(std::get<ItemCoordInventory>(location), player);
	}
}

std::unique_ptr<Item> Level::removeItem(const MapCoord& mapCoord)
{
	if (map.isMapCoordValid(mapCoord) == true)
	{
		auto item = map[mapCoord].removeObject<Item>();
		return deleteLevelObject<Item>(item);
	}
	return nullptr;
}

std::unique_ptr<Item> Level::removeItem(const ItemCoordInventory& itemCoord)
{
	Player* player;
	return removeItem(itemCoord, player);
}

std::unique_ptr<Item> Level::removeItem(const ItemCoordInventory& itemCoord, Player*& player)
{
	player = getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			return player->SelectedItem(nullptr);
		}
		std::unique_ptr<Item> nullItem;
		auto& inventory = player->getInventory(itemCoord.getInventoryIdx());
		if (itemCoord.isCoordXY() == true)
		{
			std::unique_ptr<Item> oldItem;
			inventory.set(itemCoord.getItemXY(), nullItem, oldItem);
			return oldItem;
		}
		else
		{
			std::unique_ptr<Item> oldItem;
			inventory.set(itemCoord.getItemIdx(), nullItem, oldItem);
			return oldItem;
		}
	}
	return nullptr;
}

std::unique_ptr<Item> Level::removeItem(const ItemLocation& location)
{
	Player* player;
	return removeItem(location, player);
}

std::unique_ptr<Item> Level::removeItem(const ItemLocation& location, Player*& player)
{
	if (std::holds_alternative<MapCoord>(location) == true)
	{
		player = nullptr;
		return removeItem(std::get<MapCoord>(location));
	}
	else
	{
		return removeItem(std::get<ItemCoordInventory>(location), player);
	}
}

bool Level::setItem(const MapCoord& mapCoord, std::unique_ptr<Item>& item)
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
			deleteLevelObject<Item>(oldItem);
			mapCell.removeObject(oldItem);
		}
		return true;
	}
	if (mapCell.PassableIgnoreObject(currentPlayer) == true
		&& oldItem == nullptr)
	{
		item->MapPosition(mapCoord);
		item->updateDrawPosition(*this);
		addLevelObject(std::move(item), mapCell, true);
		return true;
	}
	return false;
}

bool Level::setItem(const ItemCoordInventory& itemCoord, std::unique_ptr<Item>& item)
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

bool Level::setItem(const ItemLocation& location, std::unique_ptr<Item>& item)
{
	if (std::holds_alternative<MapCoord>(location) == true)
	{
		return setItem(std::get<MapCoord>(location), item);
	}
	else
	{
		return setItem(std::get<ItemCoordInventory>(location), item);
	}
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

void Level::deleteQuest(const std::string& questId)
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

void Level::setQuestState(const std::string& questId, int state) noexcept
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
	return experiencePoints.size();
}
